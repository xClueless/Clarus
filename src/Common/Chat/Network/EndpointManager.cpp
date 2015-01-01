#include "EndpointManager.hpp"

#include <stdexcept>
#include <iostream>
#include <QNetworkInterface>

using namespace std;

EndpointManager::EndpointManager(QString name, uint32_t port, QObject* parent) :
    QObject(parent), mLocalName(name), mPort(port), mLocalPixmap(DEFAULT_PIXMAP_URL)
{
    mServerSocket = new QTcpServer(this);
    connect(mServerSocket, SIGNAL(newConnection()), this, SLOT(newClientConnected()));

    mBroadcastSocket = new QUdpSocket(this);
    mBroadcastSocket->bind(mPort);

    connect(mBroadcastSocket, SIGNAL(readyRead()), this, SLOT(processDatagrams()));
}

QString EndpointManager::localName()
{
    return mLocalName;
}

QPixmap& EndpointManager::localPixmap()
{
    return mLocalPixmap;
}

bool EndpointManager::endpointIsConnected(QString remoteName)
{
    for(MessageEndpoint* endpoint : mIdentifiedEndpoints)
    {
        if(endpoint->socket()->peerAddress().toString() == remoteName)
        {
//			cout << "[EndpointManager] Endpoint: " << remoteName.toStdString() << " is connected" << endl;
            return true;
        }
    }
    for(MessageEndpoint* endpoint : mServersThatNeedIdentification)
    {
        if(endpoint->socket()->peerAddress().toString() == remoteName)
        {
//			cout << "[EndpointManager] Endpoint: " << remoteName.toStdString() << " is connected" << endl;
            return true;
        }
    }
    for(MessageEndpoint* endpoint : mClientsThatNeedToIdentify)
    {
        if(endpoint->socket()->peerAddress().toString() == remoteName)
        {
            cout << "[EndpointManager] Endpoint: " << remoteName.toStdString() << " is connected" << endl;
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

QList<MessageEndpoint*> EndpointManager::identifiedEndpoints()
{
    return mIdentifiedEndpoints;
}
void EndpointManager::addEnpointAsIdentified(MessageEndpoint* endpoint)
{
    mIdentifiedEndpoints << endpoint;
    endpoint->requestPixmap();
    emit endpointIdentified(endpoint);
}
void EndpointManager::serverIdentifiedUs()
{
    MessageClient* client = (MessageClient*) sender();
    mClientsThatNeedToIdentify.removeOne(client);
    addEnpointAsIdentified(client);
    cout << "[EndpointManager] Server " << client->remoteName().toStdString() << " identified us." << endl;

}

void EndpointManager::serverFailedToIdentifyUs(ConnectionError connectionError)
{
    MessageClient* client = (MessageClient*) sender();
    emit failedToConnectToEndpoint(client, connectionError);

    mClientsThatNeedToIdentify.removeOne(client);
    client->deleteLater();
}

void EndpointManager::clientIdentified()
{
    MessageServer* server = (MessageServer*) sender();
    mServersThatNeedIdentification.removeOne(server);

    cout << "[EndpointManager] Client " << server->remoteName().toStdString() << " has been identified." << endl;

    addEnpointAsIdentified(server);
}

void EndpointManager::clientFailedToIdentify(ConnectionError connectionError)
{
    MessageServer* server = (MessageServer*) sender();
    emit failedToConnectToEndpoint(server, connectionError);

    mServersThatNeedIdentification.removeOne(server);
    server->deleteLater();
}

void EndpointManager::start()
{
    if(!mServerSocket->listen(QHostAddress::Any, mPort))
    {
        //Hack for testing.
        mLocalName = "Client";
        throw runtime_error("[EndpointManager] Failed to listen to socket");
    }
}

void EndpointManager::newClientConnected()
{
    QTcpSocket* newClient = mServerSocket->nextPendingConnection();
    QString endpointString = newClient->peerAddress().toString();
    if(endpointIsConnected(endpointString))
    {
        cerr << "[EndpointManager] Client with address " << endpointString.toStdString() << " is already connected" << endl;
        cerr << "[EndpointManager] TODO Send notification of socket closure with a reason for doing so." << endl;
        newClient->close();
        delete newClient;
        return;
    }

    newClient->open(QIODevice::ReadWrite);

    MessageServer* server = new MessageServer(this, newClient);
    connect(server, SIGNAL(connectionFailed(ConnectionError)), this, SLOT(clientFailedToIdentify(ConnectionError)));
    connect(server, SIGNAL(identificationSuccessful()), this, SLOT(clientIdentified()));
    connect(server, SIGNAL(identificationFailed(ConnectionError)), this, SLOT(clientFailedToIdentify(ConnectionError)));
    mServersThatNeedIdentification << server;
}

void EndpointManager::connectToServer(QString serverHostname)
{
    if(endpointIsConnected(serverHostname))
    {
        cerr << "[EndpointManager] Client " << serverHostname.toStdString() << " is already connected. Ignoring request." << endl;
        return;
    }
    QTcpSocket* remoteServer = new QTcpSocket();

    MessageClient* mc = new MessageClient(this, remoteServer);
    connect(mc, SIGNAL(connectionFailed(ConnectionError)), this, SLOT(serverFailedToIdentifyUs(ConnectionError)));
    connect(mc, SIGNAL(identificationSuccessful()), this, SLOT(serverIdentifiedUs()));
    connect(mc, SIGNAL(identificationFailed(ConnectionError)), this, SLOT(serverFailedToIdentifyUs(ConnectionError)));
    mClientsThatNeedToIdentify.append(mc);
    mc->connectToServer(serverHostname, mPort);
}

void EndpointManager::setLocalName(QString name)
{
    mLocalName = name;
    emit localNameChanged();
}

void EndpointManager::loadLocalPixmap(QString pixmapURL)
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
            connectToServer(sender.toString());
        }
        else
        {
            cout << "[EndpointManager] Ignoring broadcast message: " << broadcastMessage.toStdString() << " from " << sender.toString().toStdString() << endl;
        }
    }
}
