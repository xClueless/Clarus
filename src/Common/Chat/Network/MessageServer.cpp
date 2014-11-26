#include "MessageServer.hpp"

#include <iostream>
#include <stdexcept>
#include <QHostAddress>
#include <QTimer>
#include "ClientManager.hpp"

using namespace std;

MessageServer::MessageServer(ClientManager* clientManager, QTcpSocket* socket, QObject* parent)
	: MessageEndpoint(clientManager, socket, parent)
{
	//Client connects to server.
	//Server asks client for identification.
	connect(mSocket, SIGNAL(connected()), this, SLOT(requestIdentity()));
	//Client identifies.
	//Client asks for identification.
}
void MessageServer::handleInternalMessage(ChatMessage* m)
{
	cout << "[MessageServer] Passing internal message down." << endl;
	MessageEndpoint::handleInternalMessage(m);
}
