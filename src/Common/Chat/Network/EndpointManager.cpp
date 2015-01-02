#include "EndpointManager.hpp"

#include <stdexcept>
#include <iostream>
#include <QNetworkInterface>
#include <QDebug>

using namespace std;

EndpointManager::EndpointManager(uint32_t port, QObject* parent) :
	QObject(parent), mPort(port)
{
	mServerSocket = new QTcpServer(this);
	connect(mServerSocket, SIGNAL(newConnection()), this, SLOT(endpointInitialisedConnection()));

	mBroadcastSocket = new QUdpSocket(this);
	mBroadcastSocket->bind(mPort);
	connect(mBroadcastSocket, SIGNAL(readyRead()), this, SLOT(processDatagrams()));
}

bool EndpointManager::endpointIsConnected(QString remoteName)
{
	for(MessageEndpoint* endpoint : mEndpoints)
	{
		if(endpoint->socket()->peerAddress().toString() == remoteName)
		{
//			cout << "[EndpointManager] Endpoint: " << remoteName.toStdString() << " is connected" << endl;
			return true;
		}
	}
//	cout << "[EndpointManager] Endpoint: " << remoteName.toStdString() << " is not connected" << endl;
	return false;
}

bool EndpointManager::isLocalAddress(QString remoteName)
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

void EndpointManager::handleConnectionFailure(ConnectionError connectionError)
{
	qDebug() << "Connection error." << connectionError.errorString();
	MessageEndpoint* me = qobject_cast<MessageEndpoint*>(sender());
	if(me != NULL)
	{
		mEndpoints.removeAll(me);
		delete me;
	}
	else
	{
		qDebug() << "Error. Failed to upcast endpoint.";
	}
}
void EndpointManager::start()
{
	if(!mServerSocket->listen(QHostAddress::Any, mPort))
	{
		//Hack for testing.
		throw runtime_error("[EndpointManager] Failed to listen to socket");
	}
}

void EndpointManager::endpointInitialisedConnection()
{
	QTcpSocket* newClient = mServerSocket->nextPendingConnection();
	QString endpointString = newClient->peerAddress().toString();
	qDebug() << "[EndpointManager] Recieved connection from endpont: " << endpointString;

	if(endpointIsConnected(endpointString))
	{
		qWarning() << "[EndpointManager] Client with address " << endpointString << " is already connected" << endl;
		qWarning() << "[EndpointManager] TODO Send notification of socket closure with a reason for doing so." << endl;
		newClient->close();
		delete newClient;
		return;
	}

	newClient->open(QIODevice::ReadWrite);

	MessageServer* server = new MessageServer(this, newClient);
	mEndpoints << server;
	connect(server, SIGNAL(connectionFailed(ConnectionError)), this, SLOT(handleConnectionFailure(ConnectionError)));
	emit endpointConnected(server);
}

void EndpointManager::connectToEndpoint(QString serverHostname)
{
	qDebug() << "[EndpointManager] Connecting to endpont: " << serverHostname;
	if(endpointIsConnected(serverHostname))
	{
		qDebug() << "[EndpointManager] Client " << serverHostname << " is already connected. Ignoring request.";
		return;
	}
	QTcpSocket* remoteServer = new QTcpSocket();

	MessageClient* mc = new MessageClient(this, remoteServer);
	mEndpoints << mc;
	connect(mc, SIGNAL(connectionFailed(ConnectionError)), this, SLOT(handleConnectionFailure(ConnectionError)));
	mc->connectToServer(serverHostname, mPort);
	emit endpointConnected(mc);
}

void EndpointManager::sendBroadcast()
{
	cout << "[EndpointManager] Sending out broadcast for local clients." << endl;
	QByteArray broadcastConnectArray = BROADCAST_CONNECT_STRING.toUtf8();
	mBroadcastSocket->writeDatagram(broadcastConnectArray, broadcastConnectArray.size(), QHostAddress::Broadcast, mPort);
//	mBroadcastSocket->write(BROADCAST_CONNECT_STRING.toUtf8());
}

void EndpointManager::processDatagrams()
{
	while (mBroadcastSocket->hasPendingDatagrams()) {
		QByteArray datagram;
		datagram.resize(mBroadcastSocket->pendingDatagramSize());
		QHostAddress sender;

		mBroadcastSocket->readDatagram(datagram.data(), datagram.size(), &sender);
		cout << "[EndpointManager] Recieved UDP datagram from " << sender.toString().toStdString() << endl;
		QString broadcastMessage = QString::fromUtf8(datagram);
		if(broadcastMessage == "CONNECT_BACK" && !endpointIsConnected(sender.toString()) && !isLocalAddress(sender.toString()))
		{
			connectToEndpoint(sender.toString());
		}
		else
		{
			cout << "[EndpointManager] Ignoring broadcast message: " << broadcastMessage.toStdString() << " from " << sender.toString().toStdString() << endl;
		}
	}
}
