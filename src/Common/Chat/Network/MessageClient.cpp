#include "MessageClient.hpp"

#include <iostream>
#include "EndpointManager.hpp"

using namespace std;

MessageClient::MessageClient(EndpointManager* endpointManager, QTcpSocket* socket, QObject* parent)
	: MessageEndpoint(endpointManager, socket, parent)
{
	//Client connects to server.
	//Server asks client for identification.
	//Client identifies.
	//Client asks for identification.
	connect(this, SIGNAL(remoteIdentifiedUs()), this, SLOT(requestIdentity()));
}

void MessageClient::connectToServer(QString clientHostname, quint16 mPort)
{
	cout << "[MessageClient] Connecting to server: " << clientHostname.toStdString() << endl;
	mSocket->connectToHost(clientHostname, mPort, QIODevice::ReadWrite);
}
