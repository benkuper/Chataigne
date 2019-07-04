/*
  ==============================================================================

    TCPServerConnectionManager.h
    Created: 4 Jul 2019 11:34:50am
    Author:  bkupe

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"

class TCPServerConnectionManager :
	public Thread
{
public:
	TCPServerConnectionManager();
	~TCPServerConnectionManager();

	StreamingSocket receiver;
	OwnedArray<StreamingSocket> connections; 
	int portToBind;

	void setupReceiver(int port);
	void removeConnection(StreamingSocket* connection);

	void close();

	class ConnectionManagerListener
	{
	public:
		virtual ~ConnectionManagerListener() {}
		virtual void receiverBindChanged(bool /*isBound*/) {}
		virtual void newConnection(StreamingSocket *) {}
	};

	ListenerList<ConnectionManagerListener> connectionManagerListeners;
	void addConnectionManagerListener(ConnectionManagerListener* newListener) { connectionManagerListeners.add(newListener); }
	void removeConnectionManagerListener(ConnectionManagerListener* listener) { connectionManagerListeners.remove(listener); }

	// Inherited via Thread
	virtual void run() override;

};