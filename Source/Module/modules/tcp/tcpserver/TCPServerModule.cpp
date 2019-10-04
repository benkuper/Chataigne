/*
  ==============================================================================

    TCPServerModule.cpp
    Created: 4 Jul 2019 10:39:27am
    Author:  bkupe

  ==============================================================================
*/

#include "TCPServerModule.h"
#include "ui/TCPServerModuleUI.h"

TCPServerModule::TCPServerModule(const String& name, int defaultLocalPort) :
	NetworkStreamingModule(name, true, false, 6000)
{
	numClients = moduleParams.addIntParameter("Num Clients", "Number of connected clients", 0, 0, 1000);
	numClients->setControllableFeedbackOnly(true);
	setupIOConfiguration(true, true);

	receiveCC->canBeDisabled = false;
	connectionManager.addConnectionManagerListener(this);

	if(!Engine::mainEngine->isLoadingFile) setupReceiver();
}

TCPServerModule::~TCPServerModule()
{
}

void TCPServerModule::setupReceiver()
{
	clearThread();
	clearInternal();
	connectionManager.close();
	
	if (Engine::mainEngine == nullptr || Engine::mainEngine->isClearing) return;
	if (!enabled->boolValue()) return;

	connectionManager.setupReceiver(localPort->intValue());
	startThread();
}

void TCPServerModule::initThread()
{
	
}

void TCPServerModule::clearThread()
{
	NetworkStreamingModule::clearThread();
	connectionManager.close();
}

bool TCPServerModule::checkReceiverIsReady()
{
	return receiverIsBound->boolValue();
}

bool TCPServerModule::isReadyToSend()
{
	return true;
}

void TCPServerModule::sendMessageInternal(const String& message)
{
	if (connectionManager.connections.size() == 0)
	{
		NLOGWARNING(niceName, "No active connections in this TCP Server, message will be lost in space");
		return;
	}

	for (auto& c : connectionManager.connections)
	{
		int numBytes = c->write(message.getCharPointer(), message.length());
		if (numBytes == -1)
		{
			NLOGERROR(niceName, "Error sending message, removing client");
			connectionManager.removeConnection(c);
			numClients->setValue(connectionManager.connections.size());
		}
	}
}

void TCPServerModule::sendBytesInternal(Array<uint8> data)
{
	if (connectionManager.connections.size() == 0)
	{
		NLOGWARNING(niceName, "No active connections in this TCP Server, message will be lost in space");
		return;
	}
	
	for (auto& c : connectionManager.connections)
	{
		int numBytes = c->write(data.getRawDataPointer(), data.size());
		if (numBytes == -1)
		{
			NLOGERROR(niceName, "Error sending data, removing client");
			connectionManager.removeConnection(c);
			numClients->setValue(connectionManager.connections.size());
		}
	}
}

Array<uint8> TCPServerModule::readBytes()
{
	Array<uint8> result;
	
	Array<StreamingSocket*> connectionsToRemove;

	for (auto& c : connectionManager.connections)
	{
		uint8 bytes[2048];

		int ready = c->waitUntilReady(true, 20);
		if (ready == -1)
		{
			connectionsToRemove.add(c);
			continue;
		}

		if (ready == 1)
		{
			int numRead = c->read(bytes, 2048, false);
			if (numRead <= 0)
			{
				connectionsToRemove.add(c);
				continue;
			}

			DBG("Num read : " << numRead);
			result.addArray(Array<uint8>(bytes, numRead)); //Not that great, all values will be stacked together. Needs proper handling
		}
	}

	for (auto& c : connectionsToRemove)
	{
		NLOGWARNING(niceName, "Connection to TCP client seems lost, removing client");
		connectionManager.removeConnection(c);
	}
	return result;
}

void TCPServerModule::clearInternal()
{
	clearThread();
	connectionManager.close();
}

void TCPServerModule::newConnection(StreamingSocket* s)
{
	numClients->setValue(connectionManager.connections.size());
	NLOG(niceName, "New Client connected : " << s->getHostName() << ":" << s->getPort());
}

void TCPServerModule::receiverBindChanged(bool isBound)
{
	receiverIsBound->setValue(isBound);
	if (isBound)
	{
		NLOG(niceName, "Server is bound on port " << localPort->intValue());
		localPort->clearWarning();
	}
	else
	{	
		String s = "Error binding to port " +  localPort->stringValue();
		NLOGERROR(niceName, s);
		localPort->setWarningMessage(s);
	}
}

ModuleUI* TCPServerModule::getModuleUI()
{
	return new TCPServerModuleUI(this);
}
