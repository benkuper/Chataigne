/*
  ==============================================================================

    Mapping2DTimeline.cpp
    Created: 23 Mar 2020 5:18:10pm
    Author:  bkupe

  ==============================================================================
*/

#include "Mapping2DLayerTimeline.h"

Mapping2DTimeline::Mapping2DTimeline(Mapping2DLayer * layer) :
	AutomationMappingLayerTimeline(layer),
	mappingLayer(layer)
{
}

void Mapping2DTimeline::paint(Graphics& g)
{
	AutomationMappingLayerTimeline::paint(g);

	if (mappingLayer->curve.keySyncMode->boolValue())
	{
		g.setColour(bgColor);
		for (auto& k : mappingLayer->curve.items)
		{
			float p = k->curvePosition / mappingLayer->curve.length->floatValue();
			g.drawHorizontalLine(getHeight() * (1 - p), 0, getWidth());
		}
	}

	if (!mappingLayer->curve.bounds.isEmpty())
	{
		Path xPath;
		Path yPath;
		xPath.startNewSubPath(0, getHeight());
		yPath.startNewSubPath(0, getHeight());
		int precision = getWidth()/3;
	
		for (int i = 0; i < precision; i++)
		{
			float p = i * 1.0f / precision;
			float t = getTimeForX(p * getWidth());
			Point<float> pos = mappingLayer->curve.getValueAtNormalizedPosition(mappingLayer->automation->getValueAtPosition(t));
			Point<float> relPos = (pos - mappingLayer->curve.bounds.getPosition()) / Point<float>(mappingLayer->curve.bounds.getWidth(), mappingLayer->curve.bounds.getHeight());
			xPath.lineTo(getXForTime(t), getHeight() * (1 - relPos.x));
			yPath.lineTo(getXForTime(t), getHeight() * (1 - relPos.y));
		}
		g.setColour(RED_COLOR);
		g.strokePath(xPath, PathStrokeType(1));
		g.setColour(GREEN_COLOR);
		g.strokePath(yPath, PathStrokeType(1));
	}
}

void Mapping2DTimeline::controllableFeedbackUpdateInternal(Controllable* c)
{
	AutomationMappingLayerTimeline::controllableFeedbackUpdateInternal(c);

	if (Curve2D * curve = ControllableUtil::findParentAs<Curve2D>(c, 2))
	{
		repaint();
	}
	else if(c == mappingLayer->curve.keySyncMode)
	{
		repaint();
	}
}
