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
	connect(this, SIGNAL(remoteIdentified()), this, SLOT(requestIdentity()));
}
void MessageServer::handleInternalMessage(ChatMessage* m)
{
	cout << "[MessageServer] Passing internal message down." << endl;
	MessageEndpoint::handleInternalMessage(m);
}
