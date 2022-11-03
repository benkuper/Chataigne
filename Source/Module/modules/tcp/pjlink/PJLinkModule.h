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
	public Module,
	public Thread,
	public Timer
{
public:
	PJLinkModule();
	~PJLinkModule();

	class PJLinkClient :
		public Thread
	{
	public:
		PJLinkClient(PJLinkModule* m, int id);
		~PJLinkClient();

		PJLinkModule* pjlinkModule;

		int id;
		StreamingSocket client;

		EnablingControllableContainer paramsCC;
		ControllableContainer valuesCC;

		StringParameter* remoteHost;
		IntParameter* remotePort;
		BoolParameter* isConnected;
		StringParameter* password;
		String passBytes;
		
		EnumParameter* powerStatus;
		BoolParameter* shutterVideoStatus;
		BoolParameter* shutterAudioStatus;

		float timeAtConnect;
		bool assigningFromRemote;

		bool lastRequestReplied;

		String stringBuffer;

		void setupClient();
		void run() override;
	};

	IntParameter* numClients;
	IntParameter* autoRequestTime;
	OwnedArray<PJLinkClient, CriticalSection> clients;

	ControllableContainer clientsParamsCC;
	ControllableContainer clientsValuesCC;

	BoolParameter* allClientsAreConnected;
	BoolParameter* allProjectorsPoweredOn;
	BoolParameter* allProjectorsPoweredOff;

	bool autoRequestIsPower;

	void updateClientsSetup();

	PJLinkClient * addClient();
	void removeClient(PJLinkClient* c);

	virtual void onControllableFeedbackUpdateInternal(ControllableContainer* cc, Controllable* c) override;

	void updateConnectedStatus();
	void updateAllPoweredStatuses();

	void sendMessageToClient(const String& message, int id = -1);

	void timerCallback() override;
	void run() override;
	void processClient(PJLinkClient* c);
	void processClientLine(PJLinkClient* c, const String& message);

	static PJLinkModule * create() { return new PJLinkModule(); }
	virtual String getDefaultTypeString() const override { return "PJLink"; }


	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PJLinkModule)
};