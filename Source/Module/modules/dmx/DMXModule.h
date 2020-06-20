/*
  ==============================================================================

    DMXModule.h
    Created: 6 Apr 2017 10:22:10pm
    Author:  Ben

  ==============================================================================
*/

#pragma once

#include "Module/Module.h"
#include "Common/DMX/device/DMXDevice.h"

enum DMXByteOrder { BIT8, MSB, LSB };

class DMXValueParameter :
	public IntParameter
{
public:
	DMXValueParameter(const String &name, const String &description, int channel, int value, const DMXByteOrder t) :
		IntParameter(name, description, value, 0, t == BIT8 ? 255 : 65535),
		type(t),
		channel(channel)
	{
	
	}

	~DMXValueParameter() {}

	var getJSONDataInternal() override
	{
		var data = IntParameter::getJSONDataInternal();
		data.getDynamicObject()->setProperty("channel", channel);
		data.getDynamicObject()->setProperty("DMXType", type);
		return data;
	}

	void loadJSONDataInternal(var data) override
	{
		Parameter::loadJSONDataInternal(data);
		channel = data.getProperty("channel", false);
		setType(static_cast<DMXByteOrder>((int)data.getProperty("DMXType", BIT8)));
	}

	void setType(DMXByteOrder t)
	{
		type = t;

		if (type != BIT8) isOverriden = true;
		setRange(0, t == BIT8 ? 255 : 65535);
		
		notifyStateChanged();
	}

	void setValueFrom2Channels(int channel1, int channel2)
	{
		setValue(type == MSB ? ((channel1 << 8) + channel2) : ((channel2 << 8) + channel1));
		DBG("Set values from channels " << channel1 << ", " << channel2 << " : " << (int)value);

	}
	

	DMXByteOrder type;
	int channel;

	ControllableUI * createDefaultUI() override;

	static DMXValueParameter* create() { return new DMXValueParameter("DMX Value Parameter", "", 0 , 1, BIT8); }
	virtual String getTypeString() const override { return getTypeStringStatic(); }
	static String getTypeStringStatic() { return "MIDI Value"; }
};


class DMXModule :
	public Module,
	public DMXDevice::DMXDeviceListener
{
public:
	DMXModule();
	~DMXModule();


	EnumParameter * dmxType;
	std::unique_ptr<DMXDevice> dmxDevice;
	BoolParameter * dmxConnected;

	Array<DMXValueParameter *> channelValues;

	//Script
	const Identifier dmxEventId = "dmxEvent";
	const Identifier sendDMXId = "send";

	void setCurrentDMXDevice(DMXDevice * d);

	void sendDMXValue(int channel, int value);
	void sendDMXValues(int channel, Array<int> values);
	void send16BitDMXValue(int startChannel, int value, DMXByteOrder byteOrder);
	void send16BitDMXValues(int startChannel, Array<int> values, DMXByteOrder byteOrder);


	//Script
	static var sendDMXFromScript(const var::NativeFunctionArgs& args);

	virtual void clearItem() override;

	var getJSONData() override;
	void loadJSONDataInternal(var data) override;

	void onContainerParameterChanged(Parameter* p) override;
	void controllableFeedbackUpdate(ControllableContainer * cc, Controllable * c) override;

	void dmxDeviceConnected() override;
	void dmxDeviceDisconnected() override;

	void dmxDataInChanged(int numChannels, uint8 * values) override;

	class DMXRouteParams :
		public RouteParams
	{
	public:
		DMXRouteParams(Module * sourceModule, Controllable * c);
		~DMXRouteParams() {}

		EnumParameter * mode16bit;
		BoolParameter * fullRange;
		IntParameter * channel;

	};

	virtual RouteParams * createRouteParamsForSourceValue(Module * sourceModule, Controllable * c, int /*index*/) override { return new DMXRouteParams(sourceModule, c); }
	virtual void handleRoutedModuleValue(Controllable * c, RouteParams * p) override;


	class DMXModuleRouterController :
		public ModuleRouterController
	{
	public:
		DMXModuleRouterController(ModuleRouter* router);

		Trigger* autoSetChannels;

		void triggerTriggered(Trigger* t) override;
	};

	ModuleRouterController* createModuleRouterController(ModuleRouter* router) override { return new DMXModuleRouterController(router); }

	static DMXModule * create() { return new DMXModule(); }
	virtual String getDefaultTypeString() const override { return "DMX"; }


	class DMXModuleListener
	{
	public:
		virtual ~DMXModuleListener() {}

		virtual void dmxDeviceChanged() {}
	};

	ListenerList<DMXModuleListener> dmxModuleListeners;
	void addDMXModuleListener(DMXModuleListener* newListener) { dmxModuleListeners.add(newListener); }
	void removeDMXModuleListener(DMXModuleListener* listener) { dmxModuleListeners.remove(listener); }
};