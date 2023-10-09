/*
  ==============================================================================

    OSCModule.h
    Created: 29 Oct 2016 7:07:07pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "servus/servus.h"
using namespace servus;

class OSCOutput :
	public BaseItem,
	public Thread
{
public:
	OSCOutput();
	~OSCOutput();

	bool forceDisabled;
	bool senderIsConnected;
	
	//SEND
	BoolParameter * useLocal;
	StringParameter * remoteHost;
	IntParameter * remotePort;
	BoolParameter* listenToOutputFeedback;
	std::unique_ptr<OSCReceiver> receiver;
	std::unique_ptr<DatagramSocket> socket;

	void setForceDisabled(bool value);

	virtual void setupSender();
	void sendOSC(const OSCMessage & m);

	virtual void run() override;

	void onContainerParameterChangedInternal(Parameter * p) override;

	virtual InspectableEditor * getEditorInternal(bool isRoot, Array<Inspectable*> inspectables = Array<Inspectable*>()) override;

private:
	OSCSender sender;
	std::queue<std::unique_ptr<OSCMessage>> messageQueue;
	CriticalSection queueLock;
};

class OSCModule :
	public Module,
	public IOSCSenderModule,
	public OSCReceiver::Listener<OSCReceiver::RealtimeCallback>,
	public Thread, //for zeroconf async creation (smoother when creating an OSC module)
	public BaseManager<OSCOutput>::ManagerListener
{ 
public:
	OSCModule(const String &name = "OSC", int defaultLocalPort = 12000, int defaultRemotePort = 9000, bool canHaveInput = true, bool canHaveOutput = true);
	~OSCModule();


	//RECEIVE
	IntParameter * localPort;
	OSCReceiver receiver;
	OSCSender genericSender;
	int defaultRemotePort;

	//ZEROCONF
	Servus servus;

	std::unique_ptr<EnablingControllableContainer> receiveCC;
	std::unique_ptr<BaseManager<OSCOutput>> outputManager;
	std::unique_ptr<ControllableContainer> thruManager;

	//Script
	const Identifier oscEventId = "oscEvent";

	//RECEIVE
	virtual void setupReceiver();

	void processMessage(const OSCMessage & msg);
	virtual void processMessageInternal(const OSCMessage &) {}

	virtual void setupModuleFromJSONData(var data) override;

	void itemAdded(OSCOutput* output) override;
	void itemsAdded(Array<OSCOutput*> outputs) override;

	//SEND
	virtual void setupSenders();
	virtual void sendOSC(const OSCMessage& msg) override;
	virtual void sendOSC(const OSCMessage& msg, String ip, int port = 0);

	//ZEROCONF
	void setupZeroConf();

	//Script
	static var sendOSCFromScript(const var::NativeFunctionArgs& args);
	static var sendOSCToFromScript(const var::NativeFunctionArgs &args);
	static var matchOSCAddrFromScript(const var::NativeFunctionArgs &args);
	static var registerOSCCallbackFromScript(const var::NativeFunctionArgs &args);

	//Thru
	static void createThruControllable(ControllableContainer* cc);


	//save / load
	virtual void setupFromManualCreation() override;

	virtual void loadJSONDataInternal(var data) override;
	virtual void afterLoadJSONDataInternal() override;

	//Routing
	class OSCRouteParams :
		public RouteParams
	{
	public:
		OSCRouteParams(Module * sourceModule, Controllable * c);
		~OSCRouteParams() {}
		StringParameter * address;
	};

	virtual RouteParams * createRouteParamsForSourceValue(Module * sourceModule, Controllable * c, int /*index*/) override { return new OSCRouteParams(sourceModule, c); }
	virtual void handleRoutedModuleValue(Controllable * c, RouteParams * p) override;
	virtual String getAddressForRoutedValue(Controllable* c, OSCRouteParams* op);

	virtual void onContainerParameterChangedInternal(Parameter * p) override;
	virtual void onContainerNiceNameChanged() override;
	virtual void onControllableFeedbackUpdateInternal(ControllableContainer * cc, Controllable * c) override;
	virtual void oscMessageReceived(const OSCMessage & message) override;
	virtual void oscBundleReceived(const OSCBundle & bundle) override;


	// Inherited via Thread
	virtual void run() override;

	//InspectableEditor * getEditor(bool isRoot) override;	

private:
	Array<std::tuple<OSCAddressPattern, Identifier>> scriptCallbacks;

};