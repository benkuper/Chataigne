/*
  ==============================================================================

	DMXUniverse.cpp
	Created: 10 Dec 2022 6:31:37pm
	Author:  bkupe

  ==============================================================================
*/

#include "Common/CommonIncludes.h"

DMXUniverse::DMXUniverse() :
	BaseItem("Universe", false, false),
	isDirty(false)
{
	editorIsCollapsed = true;

	net = addIntParameter("Net", "If appliccable the net for this universe", 0, 0, 15);
	net->hideInEditor = true;
	subnet = addIntParameter("Subnet", "If appliccable the subnet for this universe", 0, 0, 15);
	subnet->hideInEditor = true;
	universe = addIntParameter("Universe", "The universe", 0, 0);
	universe->hideInEditor = true;

	memset(values, 0, DMX_NUM_CHANNELS);

	for (int i = 0; i < DMX_NUM_CHANNELS; i++)
	{
		DMXValueParameter* p = new DMXValueParameter("Channel " + String(i + 1), "Value for this channel", i, 0, DMXByteOrder::BIT8);
		valueParams.add(p);
		addParameter(p);
	}
}

DMXUniverse::~DMXUniverse()
{

}

void DMXUniverse::updateValues(Array<uint8> newValues)
{
	jassert(newValues.size() == DMX_NUM_CHANNELS);

	for (int i = 0; i < DMX_NUM_CHANNELS; ++i)
	{
		if (DMXValueParameter* vp = valueParams[i])
		{
			if (vp->type == DMXByteOrder::BIT8) vp->setValue(values[i]);
			else if (i < DMX_NUM_CHANNELS - 1)
			{
				vp->setValueFrom2Channels(values[i], values[i + 1]);
				i++;
			}
		}
	}
}

void DMXUniverse::onContainerParameterChangedInternal(Parameter* p)
{
	if (p == net || p == subnet || p == universe) return;

	int index = valueParams.indexOf((DMXValueParameter*)p);
	jassert(index >= 0);

	GenericScopedLock lock(valueLock);
	values[index] = p->intValue();
	isDirty = true;
}

bool DMXUniverse::checkParams(int _net, int _subnet, int _universe)
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
	return "[Net : " + net->stringValue() + ", Subnet" + subnet->stringValue() + ", Universe : " + universe->stringValue();
}





ControllableUI* DMXValueParameter::createDefaultUI(Array<Controllable*> controllables)
{
	if (controllables.size() == 0) controllables = { this };
	return new DMXValueParameterUI(Inspectable::getArrayAs<Controllable, DMXValueParameter>(controllables));
}
