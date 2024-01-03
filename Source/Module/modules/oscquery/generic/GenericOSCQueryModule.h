/*
  ==============================================================================

	GenericOSCQueryModule.h
	Created: 28 Feb 2019 10:33:17pm
	Author:  bkupe

  ==============================================================================
*/

#pragma once



class GenericOSCQueryModule;
class OSCQueryOutput :
	public EnablingControllableContainer
{
public:
	OSCQueryOutput(GenericOSCQueryModule* module);
	~OSCQueryOutput();

	GenericOSCQueryModule* module;
	InspectableEditor* getEditorInternal(bool isRoot, Array<Inspectable*> inspectables = Array<Inspectable*>()) override;
};

class GenericOSCQueryModule :
	public Module,
	public IOSCSenderModule,
	public SimpleWebSocketClientBase::Listener,
	public Thread,
	public Timer,
	public EngineListener
{
public:
	GenericOSCQueryModule(const String& name = "OSCQuery", int defaultRemotePort = 5678);
	virtual ~GenericOSCQueryModule();

	const Identifier dataStructureEventId = "dataStructureEvent";

	Trigger* syncTrigger;
	BoolParameter* keepValuesOnSync;
	StringParameter* serverName;
	BoolParameter* onlySyncSameName;
	BoolParameter* useAddressForNaming;
	BoolParameter* isConnected;

	Trigger* listenAllTrigger;
	Trigger* listenNoneTrigger;

	std::unique_ptr<OSCQueryOutput> sendCC;
	BoolParameter* useLocal;
	StringParameter* remoteHost;
	IntParameter* remotePort;
	IntParameter* remoteOSCPort;
	IntParameter* remoteWSPort;

	OSCSender sender;
	std::unique_ptr<SimpleWebSocketClientBase> wsClient;
	bool isUpdatingStructure;
	bool hasListenExtension;
	var treeData; //to keep on save

	Array<Controllable*> noFeedbackList;


	void setupWSClient();

	void sendOSC(const OSCMessage& m) override;
	void sendOSCForControllable(Controllable* c);

	//Script
	static var sendOSCFromScript(const var::NativeFunctionArgs& args);

	static OSCArgument varToArgument(const var& v);

	virtual void syncData();
	virtual void updateTreeFromData(var data);

	void updateAllListens();
	void updateListenToContainer(OSCQueryHelpers::OSCQueryValueContainer* gcc, bool onlySendIfListen = false);

	virtual void onControllableFeedbackUpdateInternal(ControllableContainer* cc, Controllable* c) override;

	void connectionOpened() override;
	void connectionClosed(int status, const String& reason) override;
	void connectionError(const String& errorMessage) override;

	void dataReceived(const MemoryBlock& data) override;
	void processOSCMessage(const OSCMessage& m);
	void messageReceived(const String& message) override;

	var getJSONData() override;
	void loadJSONDataInternal(var data) override;
	void afterLoadJSONDataInternal() override;

	void endLoadFile() override;

	void timerCallback() override;

	// Inherited via Thread
	virtual void run() override;
	virtual void requestHostInfo();
	virtual void requestStructure();

	//Routing
	class OSCQueryRouteParams :
		public RouteParams,
		public Inspectable::InspectableListener
	{
	public:
		OSCQueryRouteParams(GenericOSCQueryModule* outModule, Module* sourceModule, Controllable* c);
		~OSCQueryRouteParams();

		TargetParameter* target;
		WeakReference<Controllable> cRef;

		void setControllable(Controllable* c);

		void onContainerParameterChanged(Parameter* p) override;

		void inspectableDestroyed(Inspectable* i) override;
	};

	virtual RouteParams* createRouteParamsForSourceValue(Module* sourceModule, Controllable* c, int /*index*/) override { return new OSCQueryRouteParams(this, sourceModule, c); }
	virtual void handleRoutedModuleValue(Controllable* c, RouteParams* p) override;


	static GenericOSCQueryModule* create() { return new GenericOSCQueryModule(); }
	virtual String getDefaultTypeString() const override { return "OSCQuery"; }

};
