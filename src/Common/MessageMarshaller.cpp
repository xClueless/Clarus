#include "MessageMarshaller.hpp"
#include <stdexcept>
#include <iostream>

using namespace std;

MessageMarshaller::MessageMarshaller(QString name, uint32_t port, QObject *parent) :
	QObject(parent), mName(name), mPort(port)
{
	mServerSocket = new QTcpServer();
	connect(mServerSocket, SIGNAL(newConnection()), this, SLOT(newClientConnected()));
}

QMap<QString, MessageClient*> MessageMarshaller::clients()
{
	return mClients;
}

void MessageMarshaller::start()
{
	if(!mServerSocket->listen(QHostAddress::Any, mPort))
	{
		throw runtime_error("[MessageMarshaller] Failed to listen to socket");
	}
}

void MessageMarshaller::newClientConnected()
{
	QTcpSocket* newClient = mServerSocket->nextPendingConnection();
	newClient->open(QIODevice::ReadWrite);

	cout << "Got new client." << endl;

	MessageClient* mc;
	try
	{
		mc = new MessageClient(newClient);
		mc->identify(mClients.keys());
		mClients[mc->remoteClientName()] = mc;
	}
	catch(const runtime_error& re)
	{
		if(mc)
		{
			delete mc;
		}
		cerr << "[MessageMarshaller] " << re.what() << endl;
		newClient->close();
		delete newClient;
	}
}

void MessageMarshaller::sendMessage(ChatMessage* m)
{
	for(QString targetClientString : m->targetClients())
	{
		MessageClient* targetClient = mClients[targetClientString];
		if(targetClient == NULL)
		{
			throw runtime_error("[MessageMarshaller] Client " + targetClientString.toStdString() + " not found");
		}
		targetClient->writeChatMessage(m);
	}
}

void MessageMarshaller::connectToClient(QString clientHostname)
{
	QTcpSocket* remoteClient = new QTcpSocket();
	remoteClient->connectToHost(clientHostname, mPort);

	try
	{
		MessageClient* mc = new MessageClient(remoteClient);
		mc->sendIdentify();
		mClients[clientHostname] = mc;
		mc->hook();
	}
	catch(const runtime_error& re)
	{
		cerr << "[MessageMarshaller] " << re.what() << endl;
		remoteClient->close();
		delete remoteClient;
	}
}
