#include "MessageClient.hpp"

#include <iostream>
#include "ClientManager.hpp"

using namespace std;

MessageClient::MessageClient(ClientManager* clientManager, QTcpSocket* socket, QObject* parent)
	: MessageEndpoint(clientManager, socket, parent)
{
	//Client connects to server.
	//Server asks client for identification.
	//Client identifies.
	//Client asks for identification.
	connect(this, SIGNAL(remoteIdentifiedUs()), this, SLOT(requestIdentity()));
}

void MessageClient::connectToServer(QString clientHostname, quint16 mPort)
{
	mSocket->connectToHost(clientHostname, mPort, QIODevice::ReadWrite);
}

void MessageClient::handleInternalMessage(ChatMessage* request)
{
	cout << "[MessageClient] Passing internal message down." << endl;
	MessageEndpoint::handleInternalMessage(request);
}
