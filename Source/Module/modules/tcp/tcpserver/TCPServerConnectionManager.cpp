/*
  ==============================================================================

	TCPServerConnectionManager.cpp
	Created: 4 Jul 2019 11:34:50am
	Author:  bkupe

  ==============================================================================
*/

#include "Module/ModuleIncludes.h"

TCPServerConnectionManager::TCPServerConnectionManager() :
	Thread("TCP Server Connections"),
	portToBind(0),
	queuedNotifier(10)
{
}

TCPServerConnectionManager::~TCPServerConnectionManager()
{
	close();
}

void TCPServerConnectionManager::setupReceiver(int port, const String& address)
{
	close();
	portToBind = port;
	addressToBind = address;
	startThread();
}

void TCPServerConnectionManager::removeConnection(StreamingSocket* connection)
{
	if (connection == nullptr) return;
	connections.removeObject(connection, false);

	connectionManagerListeners.call(&ConnectionManagerListener::connectionRemoved, connection);
	queuedNotifier.addMessage(new ConnectionManagerEvent(ConnectionManagerEvent::CONNECTIONS_CHANGED));

	connection->close();
	delete connection;
}

void TCPServerConnectionManager::close()
{
	if (receiver.isConnected())
	{
		receiver.close();
		while (connections.size() > 0) removeConnection(connections[0]);
	}
	stopThread(100);
}

void TCPServerConnectionManager::run()
{
	bool result = receiver.createListener(portToBind, addressToBind);
	connectionManagerListeners.call(&ConnectionManagerListener::receiverBindChanged, result);

	if (result)
	{
		while (!threadShouldExit())
		{
			StreamingSocket* socket = receiver.waitForNextConnection();
			if (socket != nullptr)
			{
				connections.add(socket);
				connectionManagerListeners.call(&ConnectionManagerListener::newConnection, socket);
				queuedNotifier.addMessage(new ConnectionManagerEvent(ConnectionManagerEvent::CONNECTIONS_CHANGED));
			}
		}
	}
	else
	{
		LOGERROR("Could not bind to port " << portToBind);
	}

	receiver.close();
}
