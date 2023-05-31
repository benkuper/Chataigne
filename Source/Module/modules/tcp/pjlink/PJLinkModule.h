/*
  ==============================================================================

	PJLinkModule.h
	Created: 5 Jan 2018 3:41:58pm
	Author:  Ben

  ==============================================================================
*/

#pragma once

#pragma once

class PJLinkModule :
	public StreamingModule,
	public Thread,
	public Timer
{
public:
	PJLinkModule();
	~PJLinkModule();

	String convertError(String numError);
	class PJLinkClient :
		public Thread,
		public ControllableContainerListener
	{
	public:
		PJLinkClient(PJLinkModule* m, int id);
		~PJLinkClient();

		PJLinkModule* pjlinkModule;

		int id;
		StreamingSocket client;

		EnablingControllableContainer paramsCC;
		ControllableContainer valuesCC;

		ControllableContainer infosCC;
		StringParameter* remoteHost;
		IntParameter* remotePort;
		BoolParameter* isConnected;
		StringParameter* password;
		String passBytes;

		EnumParameter* powerStatus;
		BoolParameter* shutterVideoStatus;
		BoolParameter* shutterAudioStatus;
		Trigger* updateInfo;
		EnumParameter* inputStatus;
		StringParameter* inputResolutionInfo;
		Trigger* updateInput;
		StringParameter* resolutionRecommendedInfo;
		StringParameter* brendInfo;
		StringParameter* productNameInfo;
		StringParameter* displayNameInfo;
		StringParameter* softVerInfo;
		StringParameter* lampHourInfo;
		StringParameter* filterUsageInfo;
		StringParameter* lampModelInfo;
		StringParameter* filterModelInfo;
		StringParameter* fanErrorInfo;
		StringParameter* lampErrorInfo;
		StringParameter* temperatureErrorInfo;
		StringParameter* coverErrorInfo;
		StringParameter* filterErrorInfo;
		StringParameter* otherErrorInfo;

		float timeAtConnect;
		bool assigningFromRemote;

		bool lastRequestReplied;

		String stringBuffer;

		void setupClient();
		void run() override;

		void controllableContainerNameChanged(ControllableContainer* cc) override;
	};

	IntParameter* numClients;
	IntParameter* autoRequestTime;
	OwnedArray<PJLinkClient, CriticalSection> clients;

	ControllableContainer clientsParamsCC;
	ControllableContainer clientsValuesCC;

	BoolParameter* allClientsAreConnected;
	BoolParameter* allProjectorsPoweredOn;
	BoolParameter* allProjectorsPoweredOff;

	StringArray inputListVp;
	StringArray chooseInput;
	int numInput, idVpInput;
	int indexInput = 0;

	const Identifier pjLinkDataReceivedId = "pjLinkDataReceived";

	bool autoRequestIsPower;

	var ghostClientNames;

	void updateClientsSetup();

	PJLinkClient* addClient();
	void removeClient(PJLinkClient* c);

	virtual void onControllableFeedbackUpdateInternal(ControllableContainer* cc, Controllable* c) override;

	void updateConnectedStatus();
	void updateAllPoweredStatuses();

	void sendMessageInternal(const String& message, var params) override;

	void sendMessageToClient(const String& message, int id = -1);

	bool isReadyToSend() override { return true; };

	void timerCallback() override;
	void run() override;
	void processClient(PJLinkClient* c);
	void processClientLine(PJLinkClient* c, const String& message);

	void requestInfos();
	void requestInputName(int id);


	var getJSONData() override;
	void loadJSONDataInternal(var data) override;

	static PJLinkModule* create() { return new PJLinkModule(); }
	virtual String getDefaultTypeString() const override { return "PJLink"; }


	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PJLinkModule)
};
