/*
  ==============================================================================

	Morpher.cpp
	Created: 11 Dec 2017 5:00:03pm
	Author:  Ben

  ==============================================================================
*/

#include "CustomVariables/CustomVariablesIncludes.h"

#define JC_VORONOI_IMPLEMENTATION
#include "jc_voronoi.h"
#include "Morpher.h"

Morpher::Morpher(CVPresetManager* presetManager) :
	ControllableContainer("Morpher"),
	Thread("Morpher"),
	presetManager(presetManager),
	mainTarget("Main"),
	attractionSleepMS(20),
	blendMode(VORONOI)
{

	diagram.reset(new jcv_diagram());

	showInspectorOnSelect = false;

	presetManager->addControllableContainerListener(this);
	presetManager->addBaseManagerListener(this);

	presetManager->showInspectorOnSelect = false;

	mainTarget.canBeDisabled = false;
	mainTarget.userCanRemove = false;
	mainTarget.userCanDuplicate = false;
	mainTarget.nameCanBeChangedByUser = false;
	mainTarget.targetColor->isSavable = false;
	mainTarget.targetColor->setColor(Colours::white);
	addChildControllableContainer(&mainTarget);

	targetPosition = addPoint2DParameter("Target Position", "Use for 2D interpolator such as Voronoi or Gradient band");

	bgImagePath = addFileParameter("Background Path", "", "");
	bgImagePath->fileTypeFilter = "*.jpg; *.jpeg; *.png; *.bmp; *.tiff";

	bgScale = addFloatParameter("Background Scale", "", 500, 10, 2000);
	bgOpacity = addFloatParameter("Background Opacity", "", 1, 0, 1);
	diagramOpacity = addFloatParameter("Diagram Opacity", "Opacity of the Voronoi Diagram", .2f, 0, 1);


	safeZone = addFloatParameter("Safe Zone", "For 2D Voronoi, this is a zone around preset for which the value will be exactly set to the preset's one", 0, 0, 1);

	useAttraction = addBoolParameter("Use Attraction", "When enabled, you can add attraction factors to targets and the MainTarget will move automatically", false);
	attractionUpdateRate = addIntParameter("Attraction update rate", "When attraction is enabled, it sets the rate at which the morpher is updating the weights", 50, 1, 200);

	attractionSpeed = addFloatParameter("Attraction speed", "Speed of the attraction. Effects depends on attraction mode", 1, .1f, 20, false);
	attractionMode = addEnumParameter("Attraction Mode", "Mode for attraction", false);
	attractionMode->addOption("Simple", SIMPLE)->addOption("Physics", PHYSICS);
	attractionMode->hideInEditor = true; //until physics is implemented

	attractionDecay = addFloatParameter("Attraction Decay", "Decay rate of the attraction on each target (1 = empty an attraction of 1 in 1s)", 0, 0, 10, false);

	showDebug = addBoolParameter("Show Debug", "Draw debug information on voronoi weights", false);

	computeZones();
}

Morpher::~Morpher()
{
	for (auto& p : presetManager->items)
	{
		p->removeControllableContainerListener(this);
	}

	presetManager->removeBaseManagerListener(this);
	presetManager->removeControllableContainerListener(this);

	stopThread(100);

	if (diagram->internal != nullptr && diagram->internal->memctx != nullptr) jcv_diagram_free(diagram.get());
}

Array<Point<float>> Morpher::getNormalizedTargetPoints()
{
	Array<Point<float>> result;
	for (CVPreset* mt : presetManager->items) if (mt->enabled->boolValue()) result.add(mt->viewUIPosition->getPoint());
	return result;
}

CVPreset* Morpher::getEnabledTargetAtIndex(int index)
{
	ScopedLock lock(presetManager->items.getLock());

	int i = 0;
	for (auto& mt : presetManager->items)
	{
		if (!mt->enabled->boolValue()) continue;
		if (i == index) return mt;
		i++;
	}
	return nullptr;
}


void Morpher::computeZones()
{
	GenericScopedLock lock(voronoiLock);

	Array<Point<float>> points = getNormalizedTargetPoints();

	if (points.size() == 0) return;

	Array<jcv_point> jPoints;

	for (Point<float> p : points)
	{
		jcv_point jp;
		jp.x = p.x;
		jp.y = p.y;
		jPoints.add(jp);

	}

	if (diagram->internal != nullptr && diagram->internal->memctx != nullptr) jcv_diagram_free(diagram.get());
	jcv_diagram_generate(points.size(), jPoints.getRawDataPointer(), nullptr, diagram.get());

	computeWeights();
}

int Morpher::getSiteIndexForPoint(Point<float> p)
{
	if (diagram == nullptr) return -1;

	if (diagram->numsites == 0) return -1;
	const jcv_site* sites = jcv_diagram_get_sites(diagram.get());

	float minDist = p.getDistanceFrom(Point<float>(sites[0].p.x, sites[0].p.y));
	float index = 0;
	for (int i = 1; i < diagram->numsites; ++i)
	{
		float dist = p.getDistanceFrom(Point<float>(sites[i].p.x, sites[i].p.y));

		if (dist < minDist)
		{
			minDist = dist;
			index = i;
		}
	}

	return index;
}


void Morpher::computeWeights()
{
	if (!voronoiLock.tryEnter()) return;

	switch (blendMode)
	{
	case VORONOI:
	{
		if (diagram->numsites <= 1) break;

		Point<float> mp = mainTarget.viewUIPosition->getPoint();

		for (CVPreset* mt : presetManager->items) mt->weight->setValue(0);

		HashMap<CVPreset*, float> rawWeights;
		float totalRawWeight = 0;


		int index = getSiteIndexForPoint(mp);
		if (index == -1) break;

		const jcv_site* sites = jcv_diagram_get_sites(diagram.get());

		jcv_site s = sites[index];

		//curZoneIndex->setValue(getSiteIndexForPoint(mp));

		float safeZ = safeZone->floatValue();

		//Compute direct site
		CVPreset* mt = getEnabledTargetAtIndex(s.index);
		float d = jmax<float>(mp.getDistanceFrom(Point<float>(s.p.x, s.p.y)) - safeZ, 0);

		if (d == 0)
		{
			mt->weight->setValue(1);
			break;
		}

		float mw = (float)INT32_MAX;
		if (d != 0) mw = 1.0f / d;
		rawWeights.set(mt, mw);
		totalRawWeight += mw;



		//Fill edge distances
		jcv_graphedge* edge = s.edges;
		Array<jcv_graphedge*> edges;

		Array<float> edgeDists;
		Array<float> edgeNeighbourDists;
		Array<jcv_site*> neighbourSites;
		Array<Line<float>> edgeLines;

		while (edge != nullptr)
		{
			jcv_site* ns = edge->neighbor;// e->sites[0]->index == s.index ? e->sites[1] : e->sites[0];

			if (ns == nullptr)
			{
				edge = edge->next;
				continue;
			}

			//DBG("Site " << s.index << " : Edge has sites " << e->sites[0]->index << "and " << e->sites[1]->index << ", chosen " << ns->index);
			//DBG("\t\t" << edge->pos[0].x << " / " << edge->pos[0].y << " / " << edge->pos[1].x << " / " << edge->pos[1].y);

			Line<float> line(Point<float>(edge->pos[0].x, edge->pos[0].y), Point<float>(edge->pos[1].x, edge->pos[1].y));
			Point<float> np;
			float distToEdge = line.getDistanceFromPoint(mp, np);
			float distNeighbourToEdge = jmax<float>(np.getDistanceFrom(Point<float>(ns->p.x, ns->p.y)) - safeZ, 0);

			edges.add(edge);

			edgeDists.add(distToEdge);
			edgeNeighbourDists.add(distNeighbourToEdge);
			neighbourSites.add(ns);
			edgeLines.add(line);

			edge = edge->next;
		}

		//DBG("Num edges to test " << edges.size());

		//Compute weight for each neighbour
		for (int i = 0; i < edges.size(); ++i)
		{
			float edgeDist = edgeDists[i];
			float edgeNDist = edgeNeighbourDists[i];
			float totalDist = edgeDist + edgeNDist;


			float minOtherEdgeDist = (float)INT32_MAX;
			jcv_graphedge* minEdge = nullptr;

			for (int j = 0; j < edges.size(); j++)
			{
				if (i == j) continue;

				//if (checkSitesAreNeighbours(edges[i]->neighbor, edges[j]->neighbor)) continue;

				float ed = edgeDists[j];
				if (ed < minOtherEdgeDist)
				{
					minOtherEdgeDist = ed;
					minEdge = edges[j];
				}
			}


			float w = 0;
			if (minEdge != nullptr)
			{
				float ratio = 1 - (edgeDist / (edgeDist + minOtherEdgeDist));
				w = ratio / totalDist;
			}
			else
			{
				float directDist = jmax<float>(mp.getDistanceFrom(Point<float>(neighbourSites[i]->p.x, neighbourSites[i]->p.y)) - safeZ, 0); //if we want to check direct distance instead of path to point
				if (directDist > 0) w = 1.0f / directDist;
				else w = (float)INT32_MAX;
			}

			CVPreset* nmt = getEnabledTargetAtIndex(neighbourSites[i]->index);
			rawWeights.set(nmt, w);
			totalRawWeight += w;
		}

		//DBG("Cur site " << s.index << " : " << mt->niceName << ": " << mw << ", total =  " << totalRawWeight << ", numWeighted neighbours " << rawWeights.size());


		//Normalize weights
		for (HashMap<CVPreset*, float>::Iterator i(rawWeights); i.next();)
		{
			float val = i.getValue() / totalRawWeight;
			i.getKey()->weight->setValue(val);

		}

	}
	break;



	case GRADIENT_BAND:
		//Not implemented yet
		break;

	default:
		break;
	}

	voronoiLock.exit();
	morpherListeners.call(&MorpherListener::weightsUpdated);
}

bool Morpher::checkSitesAreNeighbours(jcv_site* s1, jcv_site* s2)
{

	jcv_graphedge* e = s1->edges;
	while (e != nullptr)
	{
		if (e->neighbor != nullptr && e->neighbor->index == s2->index) return true;
		e = e->next;
	}
	return false;
}

void Morpher::itemAdded(CVPreset* cvp)
{
	cvp->addControllableContainerListener(this);
	computeZones();
}

void Morpher::itemsAdded(Array<CVPreset*> it)
{
	for (auto& cvp : it) cvp->addControllableContainerListener(this);
	computeZones();
}

void Morpher::itemRemoved(CVPreset* cvp)
{
	cvp->removeControllableContainerListener(this);
	computeZones();
}

void Morpher::itemsRemoved(Array<CVPreset*> it)
{
	for (auto& cvp : it) cvp->removeControllableContainerListener(this);
	computeZones();
}


void Morpher::onContainerParameterChanged(Parameter* p)
{
	if (p == targetPosition)
	{
		mainTarget.viewUIPosition->setPoint(targetPosition->getPoint());
		computeWeights();
	}
	else if (p == attractionUpdateRate)
	{
		attractionSleepMS = 1000 / attractionUpdateRate->intValue();
	}
	else if (p == useAttraction)
	{
		attractionSpeed->setEnabled(useAttraction->boolValue());
		attractionMode->setEnabled(useAttraction->boolValue());
		attractionDecay->setEnabled(useAttraction->boolValue());
		if (useAttraction->boolValue()) startThread();
		else
		{
			stopThread(100);
		}
	}
}

void Morpher::onControllableFeedbackUpdate(ControllableContainer*, Controllable* c)
{
	if (c == mainTarget.viewUIPosition)
	{
		targetPosition->setPoint(mainTarget.viewUIPosition->getPoint());
	}
	else if (MorphTarget* cvp = c->getParentAs<MorphTarget>())
	{
		if (c == cvp->viewUIPosition || c == cvp->enabled) computeZones();
	}
}

void Morpher::run()
{
	while (!threadShouldExit())
	{
		float timeFactor = attractionSleepMS / 1000.0f;

		attractionDir.setXY(0, 0);
		Point<float> mp = mainTarget.viewUIPosition->getPoint();
		int num = 0;
		for (auto& t : presetManager->items)
		{
			if (!t->enabled->boolValue()) continue;
			attractionDir += (t->viewUIPosition->getPoint() - mp) * t->attraction->floatValue();
			t->attraction->setValue(t->attraction->floatValue() - attractionDecay->floatValue() * timeFactor);
			num++;
		}

		AttractionMode am = attractionMode->getValueDataAsEnum<AttractionMode>();
		switch (am)
		{
		case SIMPLE:
			mainTarget.viewUIPosition->setPoint(mp + attractionDir * timeFactor * attractionSpeed->floatValue());
			break;

		case PHYSICS:
			break;
		}

		computeWeights();
		wait(attractionSleepMS);
	}
}
