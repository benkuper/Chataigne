
/*
  ==============================================================================

    MappingLayer.cpp
    Created: 17 Nov 2016 8:00:02pm
    Author:  Ben Kuper

  ==============================================================================
*/

#include "MappingLayer.h"

#include "../../ChataigneSequence.h"

#include "Common/Processor/Mapping/Mapping.h"
#include "Module/ModuleManager.h"

#include "ui/MappingLayerPanel.h"
#include "ui/MappingLayerTimeline.h"
#include "ui/MappingLayerEditor.h"

String MappingLayer::modeNames[] { "Mapping", "2D", "3D", "Color" };


MappingLayer::MappingLayer(Sequence *_sequence, var params) :
	SequenceLayer(_sequence, modeNames[(Mode)(int)params.getProperty("mode", MODE_1D)]),
	mode((Mode)(int)params.getProperty("mode", MODE_1D)),
	curveValue(nullptr)

{
	
	canInspectChildContainers = true;
	
	mapping.reset(new Mapping(false));
	mapping->editorIsCollapsed = false;
	mapping->editorCanBeCollapsed = false;
	mapping->hideEditorHeader = true;

	alwaysUpdate = addBoolParameter("Always Update", "If checked, the mapping will be processed and output will be sent at each time change of the sequence", false);
	sendOnPlay = addBoolParameter("Send On Play", " If checked, this will force the value to go through the mapping when sequence starts playing", true);
	sendOnStop = addBoolParameter("Send On Stop", " If checked, this will force the value to go through the mapping when sequence stops playing", true);
	sendOnSeek = addBoolParameter("Send On Seek", " If checked, this will force the value to go through the mapping when jumping time", false);

	recordSendMode = addEnumParameter("Record Send Mode", "Choose what to do when recording");
	recordSendMode->addOption("Do not send", DONOTSEND)->addOption("Send original value", SEND_ORIGINAL)->addOption("Send new value", SEND_NEW);

	addChildControllableContainer(&recorder);
	addChildControllableContainer(mapping.get());
	recorder.input->customGetTargetFunc = &ModuleManager::showAllValuesAndGetControllable;
	recorder.input->customGetControllableLabelFunc = &Module::getTargetLabelForValueControllable;
	recorder.input->customCheckAssignOnNextChangeFunc = &ModuleManager::checkControllableIsAValue;

	color->setColor(BG_COLOR.brighter(.1f));
	
	setupMappingForCurrentMode();
	uiHeight->setValue(mode == MODE_COLOR?50:120);
}

MappingLayer::~MappingLayer()
{
}

void MappingLayer::setupMappingForCurrentMode()
{
	if (curveValue != nullptr)
	{
		removeControllable(curveValue);
	}

	switch (mode)
	{
	case MODE_1D:
		curveValue = addFloatParameter("Value", "Direct curve value of the curve at the current sequence time", 0, 0, 1);
		break;
	case MODE_2D:
		curveValue = addPoint2DParameter("Value", "2D Value of the curves");
		break;
	case MODE_3D:
		curveValue = addPoint3DParameter("Value", "3D Value of the curves");
		break;

	case MODE_COLOR:
		curveValue = new ColorParameter("Value", "Color value of the curve");
		addParameter(curveValue);
		break;
	}

	if (automation != nullptr)
	{
		removeChildControllableContainer(automation.get());

		automation.reset();
	}

	if (colorManager != nullptr)
	{
		removeChildControllableContainer(colorManager.get());
		colorManager.reset();
	}

	if (mode == MODE_COLOR)
	{
		if (colorManager == nullptr)
		{
			colorManager.reset(new GradientColorManager(sequence->totalTime->floatValue(), !isCurrentlyLoadingData));
			colorManager->allowKeysOutside = false;
			colorManager->hideInEditor = true;
			addChildControllableContainer(colorManager.get());
			colorManager->setLength(sequence->totalTime->floatValue());
		}
	}
	else
	{
		automation.reset(new Automation("Automation", &recorder)); //TODO implement new multiDimension automation here
		automation->hideInEditor = true;
		automation->length->setValue(sequence->totalTime->floatValue());
		addChildControllableContainer(automation.get());
	}


	curveValue->isControllableFeedbackOnly = true;
	mapping->lockInputTo(curveValue);

	updateCurvesValues();
}

void MappingLayer::updateCurvesValues()
{
	switch (mode)
	{
	case MODE_COLOR:
		((ColorParameter*)curveValue)->setColor(colorManager->currentColor->getColor(), false);
		break;

	case MODE_1D:
	{
		if (recorder.isRecording->boolValue())
		{
			RecordSendMode m = recordSendMode->getValueDataAsEnum<RecordSendMode>();
			if (m == SEND_ORIGINAL)
			{
				curveValue->setValue(automation->value->floatValue(), false);
			}
			else if (m == SEND_NEW)
			{
				if (recorder.keys.size() > 0) curveValue->setValue(recorder.keys[recorder.keys.size() - 1].y);
			}
		}
		else
		{
			curveValue->setValue(automation->value->floatValue(), false);
		}
	}
	break;
	}
}

void MappingLayer::stopRecorderAndAddKeys()
{
	if (automation != nullptr) return;

	Array<Point<float>> keys = automation->recorder->stopRecordingAndGetKeys(); 
	if (keys.size() >= 2)
	{
		automation->addItems(keys, true, true);
	}
}

String MappingLayer::getHelpID()
{
	switch (mode)
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
	data.getDynamicObject()->setProperty("mapping", mapping->getJSONData());
	
	if(automation != nullptr) data.getDynamicObject()->setProperty(automation->shortName, automation->getJSONData());
	if (colorManager != nullptr) data.getDynamicObject()->setProperty(colorManager->shortName, colorManager->getJSONData());

	var rData = recorder.getJSONData();
	if(!rData.isVoid()) data.getDynamicObject()->setProperty("recorder", rData);

	return data;
}

void MappingLayer::loadJSONDataInternal(var data)
{
	SequenceLayer::loadJSONDataInternal(data);
	mapping->loadJSONData(data.getProperty("mapping", var()));
	if(automation != nullptr) automation->loadJSONData(data.getProperty(automation->shortName, var()));
	if (colorManager != nullptr) colorManager->loadJSONData(data.getProperty(colorManager->shortName, var()));

	recorder.loadJSONData(data.getProperty("recorder", var()));
}

void MappingLayer::exportBakedValues(bool dataOnly)
{
	double t = 0;
	double step = 1.0 / sequence->fps->floatValue();;
	
	Array<Array<float>> values;
	
	while (t <= sequence->totalTime->floatValue())
	{
		Array<float> v;
		if (mode == MODE_COLOR)
		{
			Colour c = colorManager->getColorForPosition(t);
			v.add(c.getFloatRed());
			v.add(c.getFloatGreen());
			v.add(c.getFloatBlue());
			v.add(c.getFloatAlpha());
		}
		else
		{
			v.add(automation->getValueForPosition(t));
		}
		t += step;

		values.add(v);
	}

	String s;
	for (int iv = 0; iv < values.size(); iv++)
	{
		if(s.isNotEmpty()) s += "\n";

		if(!dataOnly) s += String(iv) + "\t" + String(iv * step) + "\t";

		Array<float> va = values[iv];
		for (int i = 0; i < va.size(); i++)
		{
			s += (i > 0 ? "," : "") + String(va[i]);
		}
	}

	SystemClipboard::copyTextToClipboard(s);
	NLOG(niceName, values.size() << " keys copied to clipboard");
}

void MappingLayer::selectAll(bool addToSelection)
{
	if (mode == MODE_COLOR)
	{
		deselectThis(colorManager->items.size() == 0);
		colorManager->askForSelectAllItems(addToSelection);
	}
	else if (automation != nullptr)
	{
		deselectThis(automation->items.size() == 0);
		automation->askForSelectAllItems(addToSelection);
	}
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
	SequenceLayer::onContainerParameterChangedInternal(p);
	if (p == alwaysUpdate)
	{
		mapping->setProcessMode(alwaysUpdate->boolValue() ? Mapping::MANUAL : Mapping::VALUE_CHANGE);
	}
}

void MappingLayer::onContainerTriggerTriggered(Trigger * t)
{
	SequenceLayer::onContainerTriggerTriggered(t);
}

void MappingLayer::onControllableFeedbackUpdateInternal(ControllableContainer * cc, Controllable * c)
{
	bool doUpdate = false;
	if (mode == MODE_COLOR)
	{
		doUpdate = c == colorManager->currentColor;
	} else
	{
		doUpdate = c == automation->value;
	}

	if(doUpdate) updateCurvesValues();
}

void MappingLayer::sequenceTotalTimeChanged(Sequence *)
{
	if (mode == MODE_COLOR)
	{
		colorManager->setLength(sequence->totalTime->floatValue());
	}
	else
	{
		automation->length->setValue(sequence->totalTime->floatValue());
	}
}

void MappingLayer::sequenceCurrentTimeChanged(Sequence *, float prevTime, bool evaluateSkippedData)
{
	if (!enabled->boolValue() || !sequence->enabled->boolValue()) return;

	if (mode == MODE_COLOR)
	{
		if (colorManager == nullptr) return;
		colorManager->position->setValue(sequence->currentTime->floatValue());
	}
	else
	{
		if (automation == nullptr) return;
		automation->position->setValue(sequence->currentTime->floatValue());
	}

	if (sequence->isPlaying->boolValue())
	{
		if (automation != nullptr && automation->recorder->isRecording->boolValue())
		{
			if (prevTime < sequence->currentTime->floatValue())
			{
				automation->recorder->addKeyAt(sequence->currentTime->floatValue());
			} else
			{
				automation->recorder->startRecording();
			}
		}
	}

	if (alwaysUpdate->boolValue() || (sequence->isSeeking && sendOnSeek->boolValue()))
	{
		updateCurvesValues();
		if(automation != nullptr && !automation->items.isEmpty()) mapping->process(true); //process only if not empty
	}
}

void MappingLayer::sequencePlayStateChanged(Sequence *)
{
	if (!enabled->boolValue() || !sequence->enabled->boolValue()) return;

	if (mode == MODE_COLOR)
	{
		if (colorManager != nullptr)
		{
			if (sequence->isPlaying->boolValue())
			{
				if (sendOnPlay->boolValue()) mapping->process(true);
			}
			else
			{
				if (sendOnStop->boolValue()) mapping->process(true);
			}
		}
	}else
	{
		if (automation != nullptr)
		{

			if (sequence->isPlaying->boolValue())
			{
				if (recorder.shouldRecord()) automation->recorder->startRecording();
				if (sendOnPlay->boolValue())
				{
					updateCurvesValues();
					if(!automation->items.isEmpty()) mapping->process(true);
					
				}
			}
			else
			{
				if (automation->recorder->isRecording->boolValue())
				{
					stopRecorderAndAddKeys();
				}
				if (sendOnStop->boolValue())
				{
					updateCurvesValues();
					if (!automation->items.isEmpty()) mapping->process(true);
				}
			}
		}
	}
}

void MappingLayer::sequenceLooped(Sequence *)
{
	if (automation != nullptr && automation->recorder->isRecording->boolValue())
	{
		stopRecorderAndAddKeys();
	}
}

bool MappingLayer::paste()
{
	var data = JSON::fromString(SystemClipboard::getTextFromClipboard());
	String type = data.getProperty("itemType", "");
	if (automation != nullptr && type == automation->itemDataType)
	{
		automation->askForPaste();
		return true;
	}

	return SequenceLayer::paste();
}
