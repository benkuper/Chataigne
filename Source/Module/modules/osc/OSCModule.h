/*
  ==============================================================================

    OSCModule.h
    Created: 29 Oct 2016 7:07:07pm
    Author:  bkupe

  ==============================================================================
*/

#ifndef OSCMODULE_H_INCLUDED
#define OSCMODULE_H_INCLUDED

#include "Module/Module.h"
#include "servus/servus.h"
using namespace servus;

class OSCOutput :
	public BaseItem
{
public:
	OSCOutput();
	~OSCOutput();

	bool forceDisabled;
	
	//SEND
	BoolParameter * useLocal;
	StringParameter * remoteHost;
	IntParameter * remotePort;
	OSCSender sender;

	void setForceDisabled(bool value);

	virtual void setupSender();
	void sendOSC(const OSCMessage & m);

	void onContainerParameterChangedInternal(Parameter * p) override;
};

class OSCModule :
	public Module,
	public OSCReceiver::Listener<OSCReceiver::RealtimeCallback>,
	public Thread //for zeroconf async creation (smoother when creating an OSC module)
{ 
public:
	OSCModule(const String &name = "OSC", int defaultLocalPort = 12000, int defaultRemotePort = 9000, bool canHaveInput = true, bool canHaveOutput = true);
	~OSCModule() {}


	//RECEIVE
	IntParameter * localPort;
	BoolParameter * isConnected;
	OSCReceiver receiver;
	
	//ZEROCONF
	ScopedPointer<Servus> servus;

	ScopedPointer<EnablingControllableContainer> receiveCC;
	ScopedPointer<BaseManager<OSCOutput>> outputManager;

	//Script
	const Identifier oscEventId = "oscEvent";
	const Identifier sendOSCId = "send";

	//RECEIVE
	virtual void setupReceiver();
	float getFloatArg(OSCArgument a);
	int getIntArg(OSCArgument a);
	String getStringArg(OSCArgument a);

	void processMessage(const OSCMessage & msg);
	virtual void processMessageInternal(const OSCMessage &) {}

	virtual void setupModuleFromJSONData(var data) override;

	//SEND
	virtual void setupSenders();
	void sendOSC(const OSCMessage &msg);

	//ZEROCONF
	void setupZeroConf();

	//Script
	static var sendOSCFromScript(const var::NativeFunctionArgs &args);


	static OSCArgument varToArgument(const var &v);
	static var argumentToVar(const OSCArgument &a);


	//save / load
	virtual var getJSONData() override;
	virtual void loadJSONDataInternal(var data) override;

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

	virtual void onContainerParameterChangedInternal(Parameter * p) override;
	virtual void onContainerNiceNameChanged() override;
	virtual void onControllableFeedbackUpdateInternal(ControllableContainer * cc, Controllable * c) override;
	virtual void oscMessageReceived(const OSCMessage & message) override;
	virtual void oscBundleReceived(const OSCBundle & bundle) override;


	// Inherited via Thread
	virtual void run() override;

	//InspectableEditor * getEditor(bool isRoot) override;	
};


#endif  // OSCMODULE_H_INCLUDED
