/*
  ==============================================================================

    WebSocketClientModule.cpp
    Created: 20 Jun 2020 3:09:11pm
    Author:  bkupe

  ==============================================================================
*/

#include "WebSocketClientModule.h"

WebSocketClientModule::WebSocketClientModule(const String& name, const String &defaultServerPath) :
	StreamingModule(name)
{
	serverPath = moduleParams.addStringParameter("Server Path", "Path to the server, meaning ip:port/path", defaultServerPath);
	isConnected = moduleParams.addBoolParameter("Connected", "Is the socket sucessfully bound and listening", false);
	isConnected->setControllableFeedbackOnly(true);
	connectionFeedbackRef = isConnected;

	setupClient();
}

WebSocketClientModule::~WebSocketClientModule()
{
}

void WebSocketClientModule::setupClient()
{
	if(client != nullptr) client->stop();
	client.reset();
	if (isCurrentlyLoadingData) return;

	isConnected->setValue(false);

	if (!enabled->intValue()) return;
	client.reset(new SimpleWebSocketClient());
	client->addWebSocketListener(this);
	client->start(serverPath->stringValue());
}

bool WebSocketClientModule::isReadyToSend()
{
	return client != nullptr && isConnected->boolValue();
}

void WebSocketClientModule::sendMessageInternal(const String& message, var)
{
	client->send(message);
}

void WebSocketClientModule::sendBytesInternal(Array<uint8> data, var)
{
	//client->send((const char*)data.getRawDataPointer(), data.size());
}

void WebSocketClientModule::connectionOpened()
{
	NLOG(niceName, "Connection opened");
	isConnected->setValue(true);
}

void WebSocketClientModule::connectionClosed(int status, const String& reason)
{
	if (!enabled->boolValue()) NLOG(niceName, "Connection closed");
	isConnected->setValue(false);
}

void WebSocketClientModule::connectionError(const String& errorMessage)
{
	if(!enabled->boolValue()) NLOGERROR(niceName, "Connection error");
	isConnected->setValue(false);

}

void WebSocketClientModule::messageReceived(const String& message)
{
	if (logIncomingData->boolValue())
	{
		NLOG(niceName, "Message received :\n" << message);
	}

	StreamingType t = streamingType->getValueDataAsEnum<StreamingType>();
	switch(t)
	{
	case LINES:
	{
		StringArray sa;
		sa.addTokens(message, "\n", "\"");
		for (auto& s : sa) processDataLine(s);
	}
		break;

	case TYPE_JSON:
		processDataJSON(JSON::fromString(message));
		break;

	default:
		DBG("Not handled");
		break;
	}
}

void WebSocketClientModule::onContainerParameterChangedInternal(Parameter* p)
{
	StreamingModule::onContainerParameterChangedInternal(p);

	if (p == enabled)
	{
		if (!enabled->boolValue() && isConnected->boolValue())
		{
			NLOG(niceName, "Disabling module, closing server.");
		}

		setupClient();
	}
}

void WebSocketClientModule::onControllableFeedbackUpdateInternal(ControllableContainer* cc, Controllable* c)
{
	StreamingModule::onControllableFeedbackUpdateInternal(cc, c);

	if (c == serverPath) setupClient();
}


ModuleUI* WebSocketClientModule::getModuleUI()
{
	return StreamingModule::getModuleUI();
}
