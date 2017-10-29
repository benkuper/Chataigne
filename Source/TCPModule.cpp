/*
  ==============================================================================

    TCPModule.cpp
    Created: 21 Oct 2017 5:04:54pm
    Author:  Ben

  ==============================================================================
*/

#include "TCPModule.h"
#include "SendTCPStringCommand.h"
#include "TCPModuleEditor.h"

TCPModule::TCPModule(const String & name, int defaultLocalPort, int defaultRemotePort) :
	Module(name),
	Thread(name)
{
	canHandleRouteValues = true;

	//Receive
	localPort = addIntParameter("Local Port", "Local Port to bind to receive TCP Messages", defaultLocalPort, 1024, 65535);
	localPort->hideInOutliner = true;
	localPort->isTargettable = false;
	isBound = addBoolParameter("Is Bound", "Is the receiver bound the the local port", false);
	isBound->isEditable = false;
	isBound->hideInOutliner = true;
	isBound->isTargettable = false;
	isBound->isSavable = false;
	

	//receiver.addListener(this);
	//setupReceiver();


	//Send
	useLocal = addBoolParameter("Local", "Send to Local IP (127.0.0.1). Allow to quickly switch between local and remote IP.", true);
	remoteHost = addStringParameter("Remote Host", "Remote Host to send to.", "127.0.0.1");
	remotePort = addIntParameter("Remote port", "Port on which the remote host is listening to", defaultRemotePort, 1024, 65535);

	isConnected = addBoolParameter("Is Connected", "Is the receiver connected the the remote port", false);
	isConnected->isEditable = false;
	isConnected->hideInOutliner = true;
	isConnected->isTargettable = false;
	isConnected->isSavable = false;

	setupSender();

	reconnectRemote = addTrigger("Reconnect remote", "If remote is disconnected, tries to connect again");

	//Commands
	defManager.add(CommandDefinition::createDef(this, "", "Send string", &SendTCPStringCommand::create, CommandContext::ACTION));

	//Script
	//scriptObject.setMethod(sendOSCId, OSCModule::sendOSCFromScript);

	startThread();
}

TCPModule::~TCPModule()
{
	//
	signalThreadShouldExit();
	if(receiver.isConnected()) receiver.close();
	if (sender.isConnected()) sender.close();

	while (isThreadRunning());
}

void TCPModule::setupReceiver()
{
	//if(receiver.isConnected()) receiver.close();
	//bool result = receiver.createListener(localPort->intValue());
	//isBound->setValue(result);
}

void TCPModule::setupSender()
{
	if (sender.isConnected()) sender.close(); 
	bool result = sender.connect(useLocal->boolValue() ? "127.0.0.1" : remoteHost->stringValue(), remotePort->intValue());
	isConnected->setValue(result);
}

void TCPModule::sendStringPacket(const String &s)
{
	if (!enabled->boolValue()) return;
	if (!sender.isConnected()) setupSender();
	
	/*
	{
		DBG("Sender is not connect, can not send packet !");
		return;
	}
	*/

	DBG("Send TCP String :" << s << " > " << s.length());

	 outActivityTrigger->trigger();
	 if (logOutgoingData->boolValue())
	 {
		 String rh = useLocal->boolValue() ? "127.0.0.1" : remoteHost->stringValue();
		 NLOG(niceName, "Sending " << s << " to " << rh << ":" << remotePort->intValue());
	 }

	sender.write(s.getCharPointer(), s.length());
	//sender.close();
}

void TCPModule::onContainerParameterChangedInternal(Parameter * p)
{
	Module::onContainerParameterChangedInternal(p);
	if (p == localPort) setupReceiver();
	else if (p == remoteHost || p == remotePort || p == useLocal) setupSender();
}

void TCPModule::onContainerTriggerTriggered(Trigger * t)
{
	Module::onContainerTriggerTriggered(t);
	if (t == reconnectRemote) setupSender();
}

InspectableEditor * TCPModule::getEditor(bool isRoot)
{
	return new TCPModuleEditor(this,isRoot);
}

void TCPModule::run()
{
	sender.waitUntilReady(false, 100);
	DBG("Start run tcp loop");
	char buffer[512];

	while (!threadShouldExit())
	{
		if (sender.isConnected())
		{
			int numRead = sender.read(buffer, 512, false);
			if (numRead > 0)
			{
				DBG("Received " << numRead << "bytes");
				DBG(buffer);
				memset(buffer, 0, 512);
			}

		}
		sleep(10);
	}
}
