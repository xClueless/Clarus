#include "ClientManager.hpp"
#include <stdexcept>
#include <iostream>

using namespace std;

ClientManager::ClientManager(QString name, uint32_t port, QObject *parent) :
	QObject(parent), mName(name), mPort(port)
{
	mServerSocket = new QTcpServer();
	connect(mServerSocket, SIGNAL(newConnection()), this, SLOT(newClientConnected()));
}

QString ClientManager::localName()
{
	return mName;
}

QList<MessageEndpoint*> ClientManager::identifiedEndpoints()
{
	return mIdentifiedEndpoints;
}
void ClientManager::addEnpointAsIdentified(MessageEndpoint* endpoint)
{
	mIdentifiedEndpoints << endpoint;
	emit endpointIdentified(endpoint);
}
void ClientManager::serverIdentifiedUs()
{
	MessageClient* client = (MessageClient*) sender();
	mClientsThatNeedToIdentify.removeOne(client);
	addEnpointAsIdentified(client);
	cout << "[ClientManager] Server " << client->remoteName().toStdString() << " identified us." << endl;

}

void ClientManager::serverFailedToIdentifyUs(IdentFailure identFail)
{
	MessageClient* client = (MessageClient*) sender();
	mClientsThatNeedToIdentify.removeOne(client);
	delete client;
}

void ClientManager::clientIdentified()
{
	MessageServer* server = (MessageServer*) sender();
	mServersThatNeedIdentification.removeOne(server);

	cout << "[ClientManager] Client " << server->remoteName().toStdString() << " has been identified." << endl;

	addEnpointAsIdentified(server);
}

void ClientManager::clientFailedToIdentify(IdentFailure identFail)
{
	MessageServer* server = (MessageServer*) sender();
	mServersThatNeedIdentification.removeOne(server);
	delete server;
}

void ClientManager::start()
{
	if(!mServerSocket->listen(QHostAddress::Any, mPort))
	{
		throw runtime_error("[ClientManager] Failed to listen to socket");

		//Hack for testing.
		mName = "Client";
	}
}

void ClientManager::newClientConnected()
{
	QTcpSocket* newClient = mServerSocket->nextPendingConnection();
	newClient->open(QIODevice::ReadWrite);

	cout << "Got new client." << endl;

	MessageServer* server = new MessageServer(this, newClient);
	connect(server, SIGNAL(identificationSuccesful()), this, SLOT(clientIdentified()));
	connect(server, SIGNAL(identificationFailed(IdentFailure)), this, SLOT(clientFailedToIdentify(IdentFailure)));
	mServersThatNeedIdentification << server;
	server->requestIdentification();
}

void ClientManager::connectToServer(QString serverHostname)
{
	QTcpSocket* remoteServer = new QTcpSocket();

	MessageClient* mc = new MessageClient(this, remoteServer);
	connect(mc, SIGNAL(identificationSuccesful()), this, SLOT(serverIdentifiedUs()));
	connect(mc, SIGNAL(identificationFailed(IdentFailure)), this, SLOT(serverFailedToIdentifyUs(IdentFailure)));
	mClientsThatNeedToIdentify.append(mc);
	mc->connectToServer(serverHostname, mPort);
}
