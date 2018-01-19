
/*
  ==============================================================================

    MappingLayer.cpp
    Created: 17 Nov 2016 8:00:02pm
    Author:  Ben Kuper

  ==============================================================================
*/

#include "MappingLayer.h"
#include "MappingLayerPanel.h"
#include "MappingLayerTimeline.h"
#include "MappingLayerEditor.h"

MappingLayer::MappingLayer(Sequence *_sequence, var params) :
	SequenceLayer(_sequence, "New Automation Layer"),
	curveValue(nullptr),
	mapping(false)
{
	
	canInspectChildContainers = true;
	
	mapping.editorIsCollapsed = false;
	addChildControllableContainer(&mapping);
	addChildControllableContainer(&recorder);

	mode = addEnumParameter("Mode", "Automation Mode, 1D, 2D, 3D or Color");
	mode->addOption("Single Value", MODE_1D);
	mode->addOption("Point 2D (XY)", MODE_2D);
	mode->addOption("Point 3D (XYZ)", MODE_3D);
	mode->addOption("Color (RGBA)", MODE_COLOR);
	mode->setValueWithData((Mode)(int)params.getProperty("mode", MODE_1D));
	mode->hideInEditor = true;
	if (mode->getValueDataAsEnum<Mode>() == MODE_COLOR) setNiceName("New Color Layer");

	
	setupMappingForCurrentMode();
	uiHeight->setValue(100);
}

MappingLayer::~MappingLayer()
{
}

void MappingLayer::setupMappingForCurrentMode()
{
	Mode m = mode->getValueDataAsEnum<Mode>();
	int numAutomations = 0;

	if (curveValue != nullptr)
	{
		removeControllable(curveValue);
	}

	switch (m)
	{
	case MODE_1D:
		numAutomations = 1;
		curveValue = addFloatParameter("Value", "Direct curve value of the curve at the current sequence time", 0, 0, 1);
		break;
	case MODE_2D:
		numAutomations = 2;
		curveValue = addPoint2DParameter("Value", "2D Value of the curves");
		break;
	case MODE_3D:
		numAutomations = 3;
		curveValue = addPoint3DParameter("Value", "3D Value of the curves");
		break;

	case MODE_COLOR:
		numAutomations = 0;
		curveValue = new ColorParameter("Value", "Color value of the curve");
		addParameter(curveValue);
		break;
	}

	if (m == MODE_COLOR)
	{
		if (timeColorManager == nullptr)
		{
			timeColorManager = new TimeColorManager(sequence->totalTime->floatValue());
			addChildControllableContainer(timeColorManager);
			timeColorManager->setPositionMax(sequence->totalTime->floatValue());
		}
	}
	else
	{
		if (timeColorManager != nullptr)
		{
			removeChildControllableContainer(timeColorManager);
			timeColorManager = nullptr;
		}
	}

	while (automations.size() > numAutomations)
	{
		removeChildControllableContainer(automations.getLast());
		automations.removeLast();
	}

	for (int i = 0; i < numAutomations;i++)
	{
		if(i >= automations.size())
		{
			Automation * a = new Automation(automations.size() == 0 ? &recorder : nullptr); //only put the record on the first automation for now
			a->hideInEditor = true;
			automations.add(a);

			addChildControllableContainer(automations[i]);
		}

		automations[i]->setPositionMax(sequence->totalTime->floatValue());
	}

	curveValue->isControllableFeedbackOnly = true;
	mapping.lockInputTo(curveValue);

	updateCurvesValues();
}

void MappingLayer::updateCurvesValues()
{
	Mode mappingMode = mode->getValueDataAsEnum<Mode>();
	switch (mappingMode)
	{
	case MODE_COLOR:
		((ColorParameter *)curveValue)->setColor(timeColorManager->currentColor->getColor());
		break;

	case MODE_1D:
		curveValue->setValue(automations[0]->value->floatValue());
		break;

	case MODE_2D:
	case MODE_3D:
		var cv;
		for (auto &a : automations) cv.append(a->value->floatValue());
		curveValue->setValue(cv);
		break;
	}

}

String MappingLayer::getHelpID()
{
	Mode mappingMode = mode->getValueDataAsEnum<Mode>(); 
	switch (mappingMode)
	{
	case MODE_COLOR: return "ColorLayer";  break;

	case MODE_1D:
	case MODE_2D:
	case MODE_3D:
		return "AutomationLayer";
		break;
	}

	return "UnknownLayer";
}

var MappingLayer::getJSONData()
{
	var data = SequenceLayer::getJSONData();
	data.getDynamicObject()->setProperty("mapping", mapping.getJSONData());
	for (int i = 0; i < automations.size(); i++)
	{
		var aData = automations[i]->getJSONData();
		if(!aData.isVoid()) data.getDynamicObject()->setProperty("automation"+String(i), aData);
	}
	if (timeColorManager != nullptr)
	{
		data.getDynamicObject()->setProperty("colors", timeColorManager->getJSONData());
	}

	var rData = recorder.getJSONData();
	if(!rData.isVoid()) data.getDynamicObject()->setProperty("recorder", rData);

	return data;
}

void MappingLayer::loadJSONDataInternal(var data)
{
	SequenceLayer::loadJSONDataInternal(data);
	mapping.loadJSONData(data.getProperty("mapping", var()));
	for (int i = 0; i < automations.size(); i++)
	{
		automations[i]->loadJSONData(data.getProperty("automation"+String(i), var()));
	}
	if (timeColorManager != nullptr)
	{
		timeColorManager->loadJSONData(data.getProperty("colors", var()));
	}

	recorder.loadJSONData(data.getProperty("recorder", var()));
}

SequenceLayerPanel * MappingLayer::getPanel()
{
	return new MappingLayerPanel(this);
}

SequenceLayerTimeline * MappingLayer::getTimelineUI()
{
	return new MappingLayerTimeline(this);
}

void MappingLayer::onContainerParameterChangedInternal(Parameter * p)
{
	if (p == mode)
	{
		setupMappingForCurrentMode();
	}
}

void MappingLayer::onContainerTriggerTriggered(Trigger * t)
{
	SequenceLayer::onContainerTriggerTriggered(t);
}

void MappingLayer::onControllableFeedbackUpdateInternal(ControllableContainer * cc, Controllable * c)
{
	bool doUpdate = false;
	if (mode->getValueDataAsEnum<Mode>() == MODE_COLOR)
	{
		doUpdate = c == timeColorManager->currentColor;
	} else
	{
		for (auto &a : automations)
		{
			if (a->value == c)
			{
				doUpdate = true;
				break;
			}
		}
	}

	if(doUpdate) updateCurvesValues();
}

void MappingLayer::sequenceTotalTimeChanged(Sequence *)
{
	if (mode->getValueDataAsEnum<Mode>() == MODE_COLOR)
	{
		timeColorManager->setPositionMax(sequence->totalTime->floatValue());
	}
	else
	{
		for (auto &a : automations) a->setPositionMax(sequence->totalTime->floatValue());
	}
}

void MappingLayer::sequenceCurrentTimeChanged(Sequence *, float prevTime, bool)
{
	if (!enabled->boolValue() || !sequence->enabled->boolValue()) return;
	
	if (mode->getValueDataAsEnum<Mode>() == MODE_COLOR)
	{
		timeColorManager->position->setValue(sequence->currentTime->floatValue());
	}

	for(auto &a : automations) a->position->setValue(sequence->currentTime->floatValue());
	
	if (automations.size() > 0 && prevTime > sequence->currentTime->floatValue() && automations[0]->recorder->isRecording->boolValue())
	{
		if (sequence->isPlaying->boolValue()) automations[0]->recorder->startRecording(sequence->currentTime->floatValue());
		else automations[0]->recorder->cancelRecording();
	}

	//updateCurvesValues();
}

void MappingLayer::sequencePlayStateChanged(Sequence *)
{
	if (automations.size() > 0)
	{
		if (sequence->isPlaying->boolValue())
		{
			if(recorder.shouldRecord()) automations[0]->recorder->startRecording(sequence->currentTime->floatValue());
		}
		else
		{
			if (automations[0]->recorder->isRecording->boolValue())
			{
				Array<Point<float>> keys = automations[0]->recorder->stopRecordingAndGetKeys();
				if (keys.size() >= 2)
				{
					automations[0]->addItems(keys, true, true);
				}
			}
		}
	}
}
