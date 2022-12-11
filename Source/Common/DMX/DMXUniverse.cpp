/*
  ==============================================================================

	DMXUniverse.cpp
	Created: 10 Dec 2022 6:31:37pm
	Author:  bkupe

  ==============================================================================
*/

#include "Common/CommonIncludes.h"

DMXUniverse::DMXUniverse(bool useParams) :
	BaseItem("Universe", false, false),
	useParams(useParams),
	isDirty(false)
{
	editorIsCollapsed = !useParams;

	net = addIntParameter("Net", "If appliccable the net for this universe", 0, 0, 15);
	subnet = addIntParameter("Subnet", "If applicable the subnet for this universe", 0, 0, 15);
	universe = addIntParameter("Universe", "The universe", 0, 0);

	memset(values, 0, DMX_NUM_CHANNELS);

	if (useParams)
	{
		net->hideInEditor = true;
		subnet->hideInEditor = true;
		universe->hideInEditor = true;

		for (int i = 0; i < DMX_NUM_CHANNELS; i++)
		{
			DMXValueParameter* p = new DMXValueParameter("Channel " + String(i + 1), "Value for this channel", i, 0, DMXByteOrder::BIT8);
			valueParams.add(p);
			addParameter(p);
		}
	}
}

DMXUniverse::~DMXUniverse()
{

}

void DMXUniverse::updateValue(int channel, uint8 value)
{
	jassert(channel >= 0 && channel < DMX_NUM_CHANNELS);

	if (useParams) valueParams[channel]->setValue(value);
	else values[channel] = value;

	isDirty = true;
}

void DMXUniverse::updateValues(Array<uint8> newValues)
{
	jassert(newValues.size() == DMX_NUM_CHANNELS);

	if (useParams)
	{
		for (int i = 0; i < DMX_NUM_CHANNELS; ++i)
		{
			if (DMXValueParameter* vp = valueParams[i])
			{
				if (vp->type == DMXByteOrder::BIT8) vp->setValue(newValues[i]);
				else if (i < DMX_NUM_CHANNELS - 1)
				{
					vp->setValueFrom2Channels(newValues[i], newValues[i + 1]);
					i++;
				}
			}
		}
	}
	else
	{
		memcpy(values, newValues.getRawDataPointer(), DMX_NUM_CHANNELS);
	}

}

void DMXUniverse::onContainerParameterChangedInternal(Parameter* p)
{
	if (!useParams) return;
	if (p == net || p == subnet || p == universe) return;

	int index = valueParams.indexOf((DMXValueParameter*)p);
	jassert(index >= 0);

	GenericScopedLock lock(valueLock);
	values[index] = p->intValue();

	isDirty = true;

}

bool DMXUniverse::checkSignature(int _net, int _subnet, int _universe)
{
	return net->intValue() == _net && subnet->intValue() == _subnet && universe->intValue() == _universe;
}

InspectableEditor* DMXUniverse::getEditorInternal(bool isRoot, Array<Inspectable*> inspectables)
{
	if (inspectables.isEmpty()) inspectables.add(this);
	return new DMXUniverseEditor(inspectables, isRoot);
}

String DMXUniverse::toString() const
{
	return "[Net : " + net->stringValue() + ", Subnet" + subnet->stringValue() + ", Universe : " + universe->stringValue() + "]";
}

ControllableUI* DMXValueParameter::createDefaultUI(Array<Controllable*> controllables)
{
	if (controllables.size() == 0) controllables = { this };
	return new DMXValueParameterUI(Inspectable::getArrayAs<Controllable, DMXValueParameter>(controllables));
}
