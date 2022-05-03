/*
  ==============================================================================

	NetworkStreamingModule.cpp
	Created: 5 Jan 2018 10:42:43am
	Author:  Ben

  ==============================================================================
*/

NetworkStreamingModule::NetworkStreamingModule(const String& name, bool canHaveInput, bool canHaveOutput, int defaultLocalPort, int defaultRemotePort) :
	StreamingModule(name),
	Thread("stream_" + name),
	receiveFrequency(nullptr),
	useLocal(nullptr),
	remoteHost(nullptr),
	remotePort(nullptr),
	senderIsConnected(nullptr)
{
	setupIOConfiguration(canHaveInput, canHaveOutput);

	//Receive
	receiveFrequency = new IntParameter("Receive Frequency", "The frequency at which to receive data, only change it if you need much high frequency", 100, 1, 1000);

	if (canHaveInput)
	{
		receiveCC.reset(new EnablingControllableContainer("Input"));
		receiveCC->customGetEditorFunc = &EnablingNetworkControllableContainerEditor::create;

		moduleParams.addChildControllableContainer(receiveCC.get());

		localPort = receiveCC->addIntParameter("Local Port", "Local Port to bind", defaultLocalPort, 1, 65535);
		localPort->warningResolveInspectable = this;
		receiverIsBound = receiveCC->addBoolParameter("Is Bound", "Will be active if receiver is bound", false);
		receiverIsBound->isControllableFeedbackOnly = true;
		receiveCC->addParameter(receiveFrequency);
	}
	else
	{
		moduleParams.addParameter(receiveFrequency);
	}

	//Send
	if (canHaveOutput)
	{
		sendCC.reset(new EnablingControllableContainer("Output"));
		moduleParams.addChildControllableContainer(sendCC.get());

		useLocal = sendCC->addBoolParameter("Local", "Send to Local IP (127.0.0.1). Allow to quickly switch between local and remote IP.", true);
		remoteHost = sendCC->addStringParameter("Remote Host", "Remote Host to send to.", "127.0.0.1");
		remoteHost->setEnabled(!useLocal->boolValue());
		remoteHost->autoTrim = true;
		remotePort = sendCC->addIntParameter("Remote Port", "Port on which the remote host is listening to", defaultRemotePort, 1, 65535);

		sendCC->warningResolveInspectable = this;
		sendCC->showWarningInUI = true;
		senderIsConnected = sendCC->addBoolParameter("Is Connected", "Will be active is sender is connected", false);
		senderIsConnected->isControllableFeedbackOnly = true;
	}

	if (thruManager != nullptr)
	{
		moduleParams.controllableContainers.move(moduleParams.controllableContainers.indexOf(thruManager.get()), -1);
	}
}

NetworkStreamingModule::~NetworkStreamingModule()
{
	clearThread();
	clearInternal();
}

void NetworkStreamingModule::clearThread()
{
	stopThread(1000);
}

void NetworkStreamingModule::onContainerParameterChangedInternal(Parameter* p)
{
	if (p == enabled)
	{
		if (!isCurrentlyLoadingData)
		{
			NLOG(niceName, "Module is " << (enabled->boolValue() ? "enabled" : "disabled") << ", " << (enabled->boolValue() ? "opening" : "closing") << " connections");

			setupSender();
			setupReceiver();
		}
	}
}

void NetworkStreamingModule::onControllableFeedbackUpdateInternal(ControllableContainer* cc, Controllable* c)
{
	StreamingModule::onControllableFeedbackUpdateInternal(cc, c);
	if (c->parentContainer == sendCC.get() && !c->isControllableFeedbackOnly)
	{
		if (c == useLocal) remoteHost->setEnabled(!useLocal->boolValue());
		if (!isCurrentlyLoadingData) setupSender();
	}
	else if (c->parentContainer == receiveCC.get() && !c->isControllableFeedbackOnly)
	{
		if (!isCurrentlyLoadingData) setupReceiver();
	}
	
	if ((receiveCC != nullptr && c == receiveCC->enabled) || (sendCC != nullptr && c == sendCC->enabled))
	{
		setupIOConfiguration(canReceive(), canSend());
	}
}

bool NetworkStreamingModule::canReceive()
{
	if (receiveCC == nullptr) return false;
	return receiveCC->enabled->boolValue();
}

bool NetworkStreamingModule::canSend()
{
	if (sendCC == nullptr) return false;
	return sendCC->enabled->boolValue();
}

void NetworkStreamingModule::loadJSONDataInternal(var data)
{
	StreamingModule::loadJSONDataInternal(data);

}

void NetworkStreamingModule::afterLoadJSONDataInternal()
{
	setupSender();
	setupReceiver();
}

void NetworkStreamingModule::run()
{
	if (Engine::mainEngine != nullptr && Engine::mainEngine->isClearing) return;

	initThread();

	//char buffer[512];
	String stringBuffer = ""; //for lines;
	Array<uint8> byteBuffer; //for cobs and data255

	while (!threadShouldExit())
	{
		wait(1000 / receiveFrequency->intValue());

		runInternal();

		if (checkReceiverIsReady())
		{
			try
			{
				Array<uint8> bytes = readBytes();
				int numBytes = bytes.size();
				if (numBytes == 0) continue;

				StreamingType m = streamingType->getValueDataAsEnum<StreamingType>();
				switch (m)
				{

				case TYPE_JSON:
				{
					stringBuffer.append(String::fromUTF8((char*)bytes.getRawDataPointer(), numBytes), numBytes);
					var data = JSON::parse(stringBuffer);
					if (!data.isVoid())
					{
						processDataJSON(data);
						stringBuffer = "";
					}
				}
				break;

				case LINES:
				{
					if (CharPointer_UTF8::isValidString((char*)bytes.getRawDataPointer(), numBytes))
					{
						stringBuffer.append(String::fromUTF8((char*)bytes.getRawDataPointer(), numBytes), numBytes);
						StringArray sa;
						sa.addTokens(stringBuffer, "\r\n", "\"");
						for (int i = 0; i < sa.size() - 1; ++i) processDataLine(sa[i]);
						stringBuffer = sa[sa.size() - 1];
					}
				}
				break;

				case RAW:
				{
					processDataBytes(bytes);
				}
				break;

				case DATA255:
				{
					for (int i = 0; i < numBytes; ++i)
					{
						uint8 b = bytes[i];
						if (b == 255)
						{
							processDataBytes(byteBuffer);
							byteBuffer.clear();
						}
						else
						{
							byteBuffer.add(b);
						}
					}

				}
				break;

				case COBS:
				{
					for (int i = 0; i < numBytes; ++i)
					{
						uint8_t b = bytes[i];
						byteBuffer.add(bytes[i]);

						if (b == 0)
						{
							uint8_t decodedData[255];
							size_t numDecoded = cobs_decode(byteBuffer.getRawDataPointer(), byteBuffer.size(), decodedData);
							processDataBytes(Array<uint8>(decodedData, (int)numDecoded - 1));
							byteBuffer.clear();
						}
					}
				}
				break;
				}
			}
			catch (...)
			{
				DBG("### Streaming receive thread problem ");
			}
		}
	}
}
