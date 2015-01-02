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
}
