#include "ClientManager.hpp"

#include <stdexcept>
#include <iostream>
#include <QNetworkInterface>

using namespace std;

ClientManager::ClientManager(QString name, uint32_t port, QObject* parent) :
	QObject(parent), mLocalName(name), mPort(port), mLocalPixmap(DEFAULT_PIXMAP_URL)
{
	mServerSocket = new QTcpServer(this);
	connect(mServerSocket, SIGNAL(newConnection()), this, SLOT(newClientConnected()));

	mBroadcastSocket = new QUdpSocket(this);
	mBroadcastSocket->bind(mPort);

	connect(mBroadcastSocket, SIGNAL(readyRead()), this, SLOT(processDatagrams()));
}

QString ClientManager::localName()
{
	return mLocalName;
}

QPixmap& ClientManager::localPixmap()
{
	return mLocalPixmap;
}

bool ClientManager::endpointIsConnected(QString remoteName)
{
	for(MessageEndpoint* endpoint : mIdentifiedEndpoints)
	{
		if(endpoint->socket()->peerAddress().toString() == remoteName)
		{
//			cout << "[ClientManager] Endpoint: " << remoteName.toStdString() << " is connected" << endl;
			return true;
		}
	}
	for(MessageEndpoint* endpoint : mServersThatNeedIdentification)
	{
		if(endpoint->socket()->peerAddress().toString() == remoteName)
		{
//			cout << "[ClientManager] Endpoint: " << remoteName.toStdString() << " is connected" << endl;
			return true;
		}
	}
	for(MessageEndpoint* endpoint : mClientsThatNeedToIdentify)
	{
		if(endpoint->socket()->peerAddress().toString() == remoteName)
		{
			cout << "[ClientManager] Endpoint: " << remoteName.toStdString() << " is connected" << endl;
			return true;
		}
	}
//	cout << "[ClientManager] Endpoint: " << remoteName.toStdString() << " is not connected" << endl;
	return false;
}

bool ClientManager::isLocalAddress(QString remoteName)
{
	for(const QHostAddress& address : QNetworkInterface::allAddresses())
	{
		if(remoteName == address.toString())
		{
			return true;
		}
	}
	return false;
}

QList<MessageEndpoint*> ClientManager::identifiedEndpoints()
{
	return mIdentifiedEndpoints;
}
void ClientManager::addEnpointAsIdentified(MessageEndpoint* endpoint)
{
	mIdentifiedEndpoints << endpoint;
	endpoint->requestPixmap();
	emit endpointIdentified(endpoint);
}
void ClientManager::serverIdentifiedUs()
{
	MessageClient* client = (MessageClient*) sender();
	mClientsThatNeedToIdentify.removeOne(client);
	addEnpointAsIdentified(client);
	cout << "[ClientManager] Server " << client->remoteName().toStdString() << " identified us." << endl;

}

void ClientManager::serverFailedToIdentifyUs(ConnectionError connectionError)
{
	MessageClient* client = (MessageClient*) sender();
	emit failedToConnectToEndpoint(client, connectionError);

	mClientsThatNeedToIdentify.removeOne(client);
	client->deleteLater();
}

void ClientManager::clientIdentified()
{
	MessageServer* server = (MessageServer*) sender();
	mServersThatNeedIdentification.removeOne(server);

	cout << "[ClientManager] Client " << server->remoteName().toStdString() << " has been identified." << endl;

	addEnpointAsIdentified(server);
}

void ClientManager::clientFailedToIdentify(ConnectionError connectionError)
{
	MessageServer* server = (MessageServer*) sender();
	emit failedToConnectToEndpoint(server, connectionError);

	mServersThatNeedIdentification.removeOne(server);
	server->deleteLater();
}

void ClientManager::start()
{
	if(!mServerSocket->listen(QHostAddress::Any, mPort))
	{
		//Hack for testing.
		mLocalName = "Client";
		throw runtime_error("[ClientManager] Failed to listen to socket");
	}
}

void ClientManager::newClientConnected()
{
	QTcpSocket* newClient = mServerSocket->nextPendingConnection();
	QString endpointString = newClient->peerAddress().toString();
	if(endpointIsConnected(endpointString))
	{
		cerr << "[ClientManager] Client with address " << endpointString.toStdString() << " is already connected" << endl;
		cerr << "[ClientManager] TODO Send notification of socket closure with a reason for doing so." << endl;
		newClient->close();
		delete newClient;
		return;
	}

	newClient->open(QIODevice::ReadWrite);

	MessageServer* server = new MessageServer(this, newClient);
	connect(server, SIGNAL(connectionFailed(ConnectionError)), this, SLOT(clientFailedToIdentify(ConnectionError)));
	connect(server, SIGNAL(identificationSuccesful()), this, SLOT(clientIdentified()));
	connect(server, SIGNAL(identificationFailed(ConnectionError)), this, SLOT(clientFailedToIdentify(ConnectionError)));
	mServersThatNeedIdentification << server;
	server->requestIdentification();
}

void ClientManager::connectToServer(QString serverHostname)
{
	if(endpointIsConnected(serverHostname))
	{
		cerr << "[ClientManager] Client " << serverHostname.toStdString() << " is already connected. Ignoring request." << endl;
		return;
	}
	QTcpSocket* remoteServer = new QTcpSocket();

	MessageClient* mc = new MessageClient(this, remoteServer);
	connect(mc, SIGNAL(connectionFailed(ConnectionError)), this, SLOT(serverFailedToIdentifyUs(ConnectionError)));
	connect(mc, SIGNAL(identificationSuccesful()), this, SLOT(serverIdentifiedUs()));
	connect(mc, SIGNAL(identificationFailed(ConnectionError)), this, SLOT(serverFailedToIdentifyUs(ConnectionError)));
	mClientsThatNeedToIdentify.append(mc);
	mc->connectToServer(serverHostname, mPort);
}

void ClientManager::setLocalName(QString name)
{
	mLocalName = name;
	emit localNameChanged();
}

void ClientManager::loadLocalPixmap(QString pixmapURL)
{
	if(mLocalPixmap.load(pixmapURL, "PNG"))
	{
		emit localPixmapChanged();
	}
	else
	{
		mLocalPixmap.load(DEFAULT_PIXMAP_URL);
	}
}

void ClientManager::sendBroadcast()
{
	cout << "[ClientManager] Sending out broadcast for local clients." << endl;
	QByteArray broadcastConnectArray = BROADCAST_CONNECT_STRING.toUtf8();
	mBroadcastSocket->writeDatagram(broadcastConnectArray, broadcastConnectArray.size(), QHostAddress::Broadcast, mPort);
//	mBroadcastSocket->write(BROADCAST_CONNECT_STRING.toUtf8());
}

void ClientManager::processDatagrams()
{
	while (mBroadcastSocket->hasPendingDatagrams()) {
		QByteArray datagram;
		datagram.resize(mBroadcastSocket->pendingDatagramSize());
		QHostAddress sender;

		mBroadcastSocket->readDatagram(datagram.data(), datagram.size(), &sender);
		cout << "[ClientManager] Recieved UDP datagram from " << sender.toString().toStdString() << endl;
		QString broadcastMessage = QString::fromUtf8(datagram);
		if(broadcastMessage == "CONNECT_BACK" && !endpointIsConnected(sender.toString()) && !isLocalAddress(sender.toString()))
		{
			connectToServer(sender.toString());
		}
		else
		{
			cout << "[ClientManager] Ignoring broadcast message: " << broadcastMessage.toStdString() << " from " << sender.toString().toStdString() << endl;
		}
	}
}

