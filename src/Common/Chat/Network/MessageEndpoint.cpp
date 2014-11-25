#include "MessageEndpoint.hpp"
#include <iostream>
#include <QHostAddress>
#include "ClientManager.hpp"

using namespace std;

MessageEndpoint::MessageEndpoint(ClientManager* clientManager, QTcpSocket* socket, QObject *parent) :
	QObject(parent), mClientManager(clientManager), mSocket(socket), mNetworkStream(mSocket)
{
	connect(&mNetworkStream, SIGNAL(messageReady(QString)), this, SLOT(readChatMessage(QString)));
	connect(this, SIGNAL(internalMessageReady(ChatMessage*)), this, SLOT(processInternalMessage(ChatMessage*)));
	connect(mSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(handleSocketError(QAbstractSocket::SocketError)));
}

MessageEndpoint::~MessageEndpoint()
{
	mSocket->close();
	delete mSocket;
}

void MessageEndpoint::sendIdentError()
{
	writeInternalMessage("NOT_IDENTIFIED");
}
void MessageEndpoint::writeInternalMessage(QString messageString)
{
	ChatMessage internalMessage(MessageFlags(INTERNAL), QStringList(mRemoteName), messageString);
	writeChatMessage(&internalMessage);
}

QString MessageEndpoint::identStateString()
{
	switch(mIdentState)
	{
		case NOT_IDENTIFIED: return "NOT_IDENTIFIED";
		case CLIENT_IDENTITY_REQUESTED: return "CLIENT_IDENTITY_REQUESTED";
		case SERVER_IDENTITY_REQUESTED: return "SERVER_IDENTITY_REQUESTED";
		case CLIENT_IDENTITY_SENT: return "CLIENT_IDENTITY_SENT";
		case SERVER_IDENTITY_SENT: return "SERVER_IDENTITY_SENT";
		case IDENTIFICATION_COMPLETE: return "IDENTIFICATION_COMPLETE";
		default: return "UNKNOWN_IDENTITY_STATE";
	}
}

void MessageEndpoint::readChatMessage(QString messageString)
{
//	cout << "[MessageEndpoint] Whole message is available. Reading it now." << endl;
	try
	{
		ChatMessage* m = new ChatMessage(messageString, mRemoteName);
//		cout << "[MessageEndpoint] Message: " << m->messageString().toStdString() << endl;
		if(m->flags().type() == INTERNAL)
		{
			emit internalMessageReady(m);
		}
		else
		{
			if(mIdentState != IDENTIFICATION_COMPLETE)
			{
				sendIdentError();
			}
			else
			{
				cout << "[MessageEndpoint] Alerting subscribers to new message: " << m->message().toStdString() << endl;
				emit messageReady(m);
			}
		}
	}
	catch(const runtime_error& re)
	{
		cerr << "[MessageEndpoint] " << re.what() << endl;
	}
}

void MessageEndpoint::writeChatMessage(ChatMessage* m)
{
	mNetworkStream.writeMessage(m->messageString());
}

QString MessageEndpoint::remoteName()
{
	return mRemoteName;
}

void MessageEndpoint::processInternalMessage(ChatMessage* m)
{
	cerr << "[MessageEndpoint] Internal message: " << m->message().toStdString()
		 << " not understood in current state: " << identStateString().toStdString() << endl;
	writeInternalMessage("UNKNOWN_COMMAND");
	while(true)
	{

	}
}

void MessageEndpoint::setRemoteName(QString name)
{
	mRemoteName = name;
	emit remoteNameChanged();
}

void MessageEndpoint::handleSocketError(QAbstractSocket::SocketError error)
{
	emit connectionFailed(ConnectionError(mSocket, error));
}

