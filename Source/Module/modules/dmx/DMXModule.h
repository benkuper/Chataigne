/*
  ==============================================================================

	DMXModule.h
	Created: 6 Apr 2017 10:22:10pm
	Author:  Ben

  ==============================================================================
*/

#pragma once





class DMXModule :
	public Module,
	public DMXDevice::DMXDeviceListener
{
public:
	DMXModule();
	~DMXModule();


	EnumParameter* dmxType;
	std::unique_ptr<DMXDevice> dmxDevice;
	BoolParameter* dmxConnected;

	Array<DMXValueParameter*> channelValues;

	//Script
	const Identifier dmxEventId = "dmxEvent";
	const Identifier sendDMXId = "send";

	std::unique_ptr<ControllableContainer> thruManager;

	DMXUniverseManager inputUniverseManager;
	DMXUniverseManager outputUniverseManager;

	void setCurrentDMXDevice(DMXDevice* d);

	void sendDMXValue(DMXUniverse* u, int channel, uint8 value);
	void sendDMXRange(DMXUniverse* u, int startChannel, Array<uint8> values);
	void send16BitDMXValue(DMXUniverse* u, int channel, int value, DMXByteOrder byteOrder);
	void send16BitDMXRange(DMXUniverse* u, int startChannel, Array<int> values, DMXByteOrder byteOrder);


	//Script
	static var sendDMXFromScript(const var::NativeFunctionArgs& args);

	virtual void clearItem() override;

	var getJSONData() override;
	void loadJSONDataInternal(var data) override;

	void onContainerParameterChanged(Parameter* p) override;
	void controllableFeedbackUpdate(ControllableContainer* cc, Controllable* c) override;

	void dmxDeviceConnected() override;
	void dmxDeviceDisconnected() override;

	void dmxDataInChanged(int net, int subnet, int universe, Array<uint8> values, const String& sourceName = "") override;

	DMXUniverse* getUniverse(bool isInput, int net, int subnet, int universe, bool createIfNotThere = true);

	static void createThruControllable(ControllableContainer* cc);

	class DMXRouteParams :
		public RouteParams
	{
	public:
		DMXRouteParams(Module* sourceModule, Controllable* c);
		~DMXRouteParams() {}

		EnumParameter* mode16bit;
		BoolParameter* fullRange;
		IntParameter* channel;

	};

	virtual RouteParams* createRouteParamsForSourceValue(Module* sourceModule, Controllable* c, int /*index*/) override { return new DMXRouteParams(sourceModule, c); }
	virtual void handleRoutedModuleValue(Controllable* c, RouteParams* p) override;


	class DMXModuleRouterController :
		public ModuleRouterController
	{
	public:
		DMXModuleRouterController(ModuleRouter* router);

		Trigger* autoSetChannels;

		void triggerTriggered(Trigger* t) override;
	};

	ModuleRouterController* createModuleRouterController(ModuleRouter* router) override { return new DMXModuleRouterController(router); }

	static DMXModule* create() { return new DMXModule(); }
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