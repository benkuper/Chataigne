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
	streamingType->clearOptions();
	streamingType->addOption("Lines", LINES);
	streamingType->addOption("JSON", TYPE_JSON);

	useSecureConnection = moduleParams.addBoolParameter("Use Secure Connection", "If checked, this will use a secure connection. Use this if the server you're connecting to is using wss://", false);
	serverPath = moduleParams.addStringParameter("Server Path", "Path to the server, meaning ip:port/path WITHOUT ws:// or wss://", defaultServerPath);
	isConnected = moduleParams.addBoolParameter("Connected", "Is the socket sucessfully bound and listening", false);
	isConnected->setControllableFeedbackOnly(true);
	connectionFeedbackRef = isConnected;

	scriptManager->scriptTemplate += ChataigneAssetManager::getInstance()->getScriptTemplate("wsClient");

	startTimer(1000);
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

	if (useSecureConnection->boolValue())
	{
#if SIMPLEWEB_SECURE_SUPPORTED
		client.reset(new SecureWebSocketClient());
#else
		NLOGWARNING(niceName, "Secure connection is only supported on Windows right now.");
		client.reset(new SimpleWebSocketClient());
#endif
	}
	else client.reset(new SimpleWebSocketClient());

	client->addWebSocketListener(this);
	client->start(serverPath->stringValue());
	}

	bool WebSocketClientModule::isReadyToSend()
	{
		return client != nullptr && isConnected->boolValue();
	}

	void WebSocketClientModule::sendMessageInternal(const String & message, var params)
	{
		client->send(message);
	}

	void WebSocketClientModule::sendBytesInternal(Array<uint8> data, var params)
	{
		client->send((const char*)data.getRawDataPointer(), data.size());
	}

	void WebSocketClientModule::connectionOpened()
	{
		NLOG(niceName, "Connection opened");
		isConnected->setValue(true);
	}

	void WebSocketClientModule::connectionClosed(int status, const String & reason)
	{
		NLOG(niceName, "Connection closed");
		isConnected->setValue(false);
	}

	void WebSocketClientModule::connectionError(const String & errorMessage)
	{
		if (enabled->boolValue() && connectFirstTry) NLOGERROR(niceName, "Connection error " << errorMessage);
		isConnected->setValue(false);

	}

	void WebSocketClientModule::messageReceived(const String & message)
	{
		if (!enabled->boolValue()) return;
		scriptManager->callFunctionOnAllItems(wsMessageReceivedId, message);

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
			//DBG("Not handled");
			break;
		}
	}

	void WebSocketClientModule::dataReceived(const MemoryBlock & data)
	{
		inActivityTrigger->trigger();

		Array<uint8_t> bytes((const uint8_t*)data.getData(), data.getSize());

		if (logIncomingData->boolValue())
		{
			String s = "";
			for (auto& b : bytes) s += String(b) + "\n";
			NLOG(niceName, "Received " << bytes.size() << " bytes :\n" << s);
		}

		Array<var> args;
		var bytesData;
		for (auto& b : bytes) bytesData.append(b);
		args.add(bytesData);
		scriptManager->callFunctionOnAllItems(wsDataReceivedId, args);
	}

	void WebSocketClientModule::onContainerParameterChangedInternal(Parameter * p)
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

	void WebSocketClientModule::onControllableFeedbackUpdateInternal(ControllableContainer * cc, Controllable * c)
	{
		StreamingModule::onControllableFeedbackUpdateInternal(cc, c);

		if (c == serverPath)
		{
			connectFirstTry = true;

			String s = serverPath->stringValue();
			if (s.startsWith("ws://"))
			{
				serverPath->setValue(s.substring(5));
				return;
			}
			else if (s.startsWith("wss://"))
			{
				serverPath->setValue(s.substring(6));
				useSecureConnection->setValue(true);
				return;
			}

			setupClient();
		}
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
		else if (c == useSecureConnection)
		{
			if (!isCurrentlyLoadingData)
			{
				connectFirstTry = true;
				setupClient();
			}
		}
	}

	void WebSocketClientModule::timerCallback()
	{
		if (!isConnected->boolValue())
		{
			setupClient();

			if (connectFirstTry) startTimer(5000);
			connectFirstTry = false;
		}
	}

	void WebSocketClientModule::afterLoadJSONDataInternal()
	{
		StreamingModule::afterLoadJSONDataInternal();
		setupClient();
	}


	ModuleUI* WebSocketClientModule::getModuleUI()
	{
		return StreamingModule::getModuleUI();
	}
