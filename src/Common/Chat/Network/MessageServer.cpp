#include "MessageServer.hpp"

#include <iostream>
#include <stdexcept>
#include <QHostAddress>
#include <QTimer>
#include "EndpointManager.hpp"

using namespace std;

MessageServer::MessageServer(EndpointManager* endpointManager, QTcpSocket* socket, QObject* parent)
	: MessageEndpoint(endpointManager, socket, parent)
{
	//Client has already connected at this point.
	//Server asks client for identification.
	mIdentityExchange->requestIdentity();
	//Client identifies.
	//Client asks for identification.
}

void MessageServer::handleInternalMessage(ChatMessage* m)
{
	cout << "[MessageServer] Passing internal message down." << endl;
	MessageEndpoint::handleInternalMessage(m);
}
