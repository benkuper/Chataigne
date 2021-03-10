/*
  ==============================================================================

	WebSocketClientModule.cpp
	Created: 20 Jun 2020 3:09:11pm
	Author:  bkupe

  ==============================================================================
*/

WebSocketClientModule::WebSocketClientModule(const String& name, const String& defaultServerPath) :
	StreamingModule(name),
	connectFirstTry(true)
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
	if (client != nullptr) client->stop();
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
	NLOG(niceName, "Connection closed");
	isConnected->setValue(false);
}

void WebSocketClientModule::connectionError(const String& errorMessage)
{
	if (enabled->boolValue() && connectFirstTry) NLOGERROR(niceName, "Connection error " << errorMessage);
	isConnected->setValue(false);

}

void WebSocketClientModule::messageReceived(const String& message)
{
	StreamingType t = streamingType->getValueDataAsEnum<StreamingType>();
	switch (t)
	{
	case LINES:
	{
		StringArray sa;
		sa.addTokens(message, "\n", "\"");
		for (auto& s : sa) processDataLine(s);
	}
	break;

	case TYPE_JSON:
	{
		var result;
		juce::Result r = JSON::parse(message, result);
		if (r.failed()) NLOGWARNING(niceName, "Error parsing message :\n" << r.getErrorMessage());
		processDataJSON(result);
	}
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

		connectFirstTry = true;
		setupClient();
	}
}

void WebSocketClientModule::onControllableFeedbackUpdateInternal(ControllableContainer* cc, Controllable* c)
{
	StreamingModule::onControllableFeedbackUpdateInternal(cc, c);

	if (c == serverPath) setupClient();
	else if (c == isConnected)
	{

		if (isConnected->boolValue())
		{
			stopTimer();
		}
		else
		{
			connectFirstTry = true;
			startTimer(1000);
		}
	}
}

void WebSocketClientModule::timerCallback()
{
	if (!isConnected->boolValue())
	{
		setupClient();
		connectFirstTry = false;
	}
}


ModuleUI* WebSocketClientModule::getModuleUI()
{
	return StreamingModule::getModuleUI();
}
