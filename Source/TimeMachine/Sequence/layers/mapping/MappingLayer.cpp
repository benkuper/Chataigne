
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

MappingLayer::MappingLayer(const String &name, Sequence *_sequence, var params) :
	SequenceLayer(_sequence, name),
	mappingInputSource(nullptr),
	mappingInput(nullptr)
{
	canInspectChildContainers = true;
	saveAndLoadRecursiveData = true;
	
	mapping.reset(new Mapping(false));
	mapping->editorIsCollapsed = false;
	mapping->editorCanBeCollapsed = false;
	mapping->hideEditorHeader = true;

	alwaysUpdate = addBoolParameter("Always Update", "If checked, the mapping will be processed and output will be sent at each time change of the sequence", false);
	sendOnPlay = addBoolParameter("Send On Play", " If checked, this will force the value to go through the mapping when sequence starts playing", true);
	sendOnStop = addBoolParameter("Send On Stop", " If checked, this will force the value to go through the mapping when sequence stops playing", true);
	sendOnSeek = addBoolParameter("Send On Seek", " If checked, this will force the value to go through the mapping when jumping time", false);

	addChildControllableContainer(mapping.get());
	
	color->setColor(BG_COLOR.brighter(.1f));
	
}

MappingLayer::~MappingLayer()
{
}

void MappingLayer::setupMappingInputParameter(Parameter* source)
{
	jassert(mappingInput == nullptr);
	mappingInputSource = source;
	mappingInput = ControllableFactory::createParameterFrom(source, true, true);
	addParameter(mappingInput);

	mapping->lockInputTo(mappingInput);
	updateMappingInputValue();
}

void MappingLayer::updateMappingInputValue(bool forceOutput)
{
	if (!enabled->boolValue() || !sequence->enabled->boolValue()) return;
	updateMappingInputValueInternal();
	if (forceOutput || alwaysUpdate->boolValue()) mapping->process(true);
}

void MappingLayer::updateMappingInputValueInternal()
{
	mappingInput->setValue(mappingInputSource->value);
}

void MappingLayer::exportBakedValues(bool dataOnly)
{
	double t = 0;
	double step = 1.0 / sequence->fps->floatValue();;
	
	Array<Array<float>> values;
	
	while (t <= sequence->totalTime->floatValue())
	{
		Array<float> pValues;
		var v = getValueAtPosition(t);
		if (v.isArray())
		{
			for (int i = 0; i < v.size(); i++) pValues.add(v[i]);
		}
		else pValues.add((float)v);

		t += step;

		values.add(pValues);
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
	SequenceLayer::onControllableFeedbackUpdateInternal(cc, c);
	if (c == mappingInputSource) updateMappingInputValue();
}

void MappingLayer::sequenceCurrentTimeChanged(Sequence * s, float prevTime, bool evaluateSkippedData)
{
	if (!enabled->boolValue() || !sequence->enabled->boolValue()) return;

	sequenceCurrentTimeChangedInternal(s, prevTime, evaluateSkippedData);

	if (alwaysUpdate->boolValue() || (sequence->isSeeking && sendOnSeek->boolValue()))
	{
		updateMappingInputValue(true);
	}
}

void MappingLayer::sequencePlayStateChanged(Sequence * s)
{
	if (!enabled->boolValue() || !sequence->enabled->boolValue()) return;

	sequencePlayStateChangedInternal(s);

	bool updateAndProcess = (sequence->isPlaying->boolValue() && sendOnPlay->boolValue()) || (!sequence->isPlaying->boolValue() && sendOnStop->boolValue());
	if (updateAndProcess) updateMappingInputValue(true);
}