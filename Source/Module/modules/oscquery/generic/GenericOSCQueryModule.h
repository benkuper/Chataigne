/*
  ==============================================================================

    GenericOSCQueryModule.h
    Created: 28 Feb 2019 10:33:17pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "Module/Module.h"

class GenericOSCQueryModule;
class OSCQueryOutput :
	public EnablingControllableContainer
{
public:
	OSCQueryOutput(GenericOSCQueryModule * module);
	~OSCQueryOutput();

	GenericOSCQueryModule * module;
	InspectableEditor * getEditor(bool isRoot) override;
};

class GenericOSCQueryModule :
	public Module,
	public Thread
{
public:
	GenericOSCQueryModule(const String &name = "OSCQuery",int defaultRemotePort = 5678);
	virtual ~GenericOSCQueryModule();

	const Identifier dataStructureEventId = "dataStructureEvent";

	Trigger * syncTrigger;

	std::unique_ptr<OSCQueryOutput> sendCC;
	BoolParameter * useLocal;
	StringParameter * remoteHost;
	IntParameter * remotePort;
	IntParameter* remoteOSCPort;

	OSCSender sender;

	SpinLock dataLock;
	ValueTree dataTree;

	OwnedArray<ControllableContainer> valuesContainers;

	void sendOSCMessage(OSCMessage m);
	void sendOSCForControllable(Controllable * c);
	
	//Script
	static var sendOSCFromScript(const var::NativeFunctionArgs &args);
	
	static OSCArgument varToArgument(const var &v);

	virtual void syncData();
	virtual void createTreeFromData(var data);
	virtual void fillContainerFromData(ControllableContainer * cc, var data);
	virtual Controllable * createControllableFromData(StringRef name, var data);

	virtual void onControllableFeedbackUpdateInternal(ControllableContainer * cc, Controllable * c) override;

	static GenericOSCQueryModule * create() { return new GenericOSCQueryModule(); }
	virtual String getDefaultTypeString() const override { return "OSCQuery"; }


	// Inherited via Thread
	virtual void run() override;

};