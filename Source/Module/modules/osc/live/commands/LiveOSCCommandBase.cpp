/*
  ==============================================================================

    LiveOSCCommandBase.cpp
    Created: 13 Apr 2017 11:28:34am
    Author:  Ben

  ==============================================================================
*/

#include "LiveOSCCommandBase.h"



LiveOSCCommandBase::LiveOSCCommandBase(LiveOSCModule* _module, CommandContext context, var params, IteratorProcessor * iterator) :
	OSCCommand(_module, context, params),
	liveModule(_module),
	trackID(nullptr),
	clipID(nullptr),
	deviceID(nullptr),
	paramID(nullptr),
	value(nullptr)
{

	addressModel = params.getProperty("addressModel", "");

	Controllable::Type type = (Controllable::Type)(int)params.getProperty("type", Controllable::TRIGGER);
	
	if (type != Controllable::Type::TRIGGER)
	{
		value = (Parameter*)ControllableFactory::createControllable(Controllable::typeNames[type]);
		value->setNiceName("Value");
		value->setRange(params.getProperty("min", INT32_MIN), params.getProperty("max", INT32_MAX));
		if (params.hasProperty("default"))
		{
			value->defaultValue = params.getProperty("default", value->value);
			value->resetValue();
		}

		if (params.hasProperty("ui")) ((FloatParameter*)value)->defaultUI = (FloatParameter::UIType)(int)params.getProperty("ui", ((FloatParameter*)value)->defaultUI);

		argumentsContainer.addControllable(value);

		//addTargetMappingParameterAt(value, 0);
	}
	
	if (addressModel.contains("[scene]")) paramID = addIntParameter("Scene", "Index of the scene, 0 is first scene", 0, 0);
	if (addressModel.contains("[track]")) trackID = addIntParameter("Track", "Index of the target track, 0 is first track", 0, 0);
	if (addressModel.contains("[clip]")) clipID = addIntParameter("Clip", "Index of the clip, 0 is first clip", 0, 0);
	if (addressModel.contains("[device]")) clipID = addIntParameter("Device", "Index of the device in the track, 0 is first device", 0, 0);
	if (addressModel.contains("[parameter]")) paramID = addIntParameter("Parameter", "Index of the clip, 0 is first clip", 0, 0);

	rebuildAddressOnParamChanged = true;
	rebuildAddress();
}

LiveOSCCommandBase::~LiveOSCCommandBase()
{
}