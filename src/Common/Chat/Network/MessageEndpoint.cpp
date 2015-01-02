#include "MessageEndpoint.hpp"

#include <QHostAddress>
#include <QBuffer>
#include <QDebug>
#include "EndpointManager.hpp"
#include "../Resource/ResourceMessage.hpp"

using namespace std;

MessageEndpoint::MessageEndpoint(EndpointManager* endpointManager, QTcpSocket* socket, QObject *parent) :
	QObject(parent), mEndpointManager(endpointManager), mSocket(socket), mNetworkStream(mSocket)
{
	connect(mSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(handleSocketError(QAbstractSocket::SocketError)));
	connect(&mNetworkStream, SIGNAL(messageReady(QByteArray)), this, SLOT(readChatMessage(QByteArray)));
	connect(mSocket, SIGNAL(connected()), this, SLOT(connected()));

	connect(mRemoteIdentity.name(), SIGNAL(writeChatMessage(ChatMessage*)), this, SLOT(writeChatMessage(ChatMessage*)));
	connect(mRemoteIdentity.picture(), SIGNAL(writeChatMessage(ChatMessage*)), this, SLOT(writeChatMessage(ChatMessage*)));

	mSharedResources[mRemoteIdentity.name()->resourceName()] = mRemoteIdentity.name();
	mSharedResources[mRemoteIdentity.picture()->resourceName()] = mRemoteIdentity.picture();
}

MessageEndpoint::~MessageEndpoint()
{
	mSocket->close();
	delete mSocket;
}

RemoteIdentity* MessageEndpoint::identity()
{
	return &mRemoteIdentity;
}

void MessageEndpoint::readChatMessage(QByteArray messageBytes)
{
//	cout << "[MessageEndpoint] Whole message is available. Reading it now." << endl;
	try
	{
		QJsonDocument json = QJsonDocument::fromJson(messageBytes);
		qDebug() << "[MessageEndpoint] Recieved JSON: " << json;
		ChatMessage* m = new ChatMessage(json, "");
//		cout << "[MessageEndpoint] Message: " << m->messageString().toStdString() << endl;
		if(m->type() == ChatMessage::MessageType::RESOURCE_EXCHANGE)
		{
			ResourceMessage rm(m);
			if(!rm.resourceName().isEmpty())
			{
				mSharedResources[m->section("ResourceName").toString()]->handleResourceMessage(&rm);
			}
			else
			{
				qCritical() << "[MessageEndpoint] RESOURCE_EXCHANGE message does not contain a ResourceName field.";
			}
		}
		else if(m->type() == ChatMessage::MessageType::PRIVATE)
		{
			qDebug() << "[MessageEndpoint] Alerting subscribers to new message.";
			emit messageReady(m);
		}
		else
		{
			qDebug() << "[MessageEndpoint] Error. Message not understood. We don't know how to handle the message type.";
		}
	}
	catch(const runtime_error& re)
	{
		qDebug() << "[MessageEndpoint::readChatMessage(QByteArray)] Error: " << re.what() << endl;
	}
}

void MessageEndpoint::writeChatMessage(ChatMessage* m)
{
	qDebug() << "[MessageEndpoint] Writing message:";
	qDebug() << m->jsonDocument();
	mNetworkStream.writeMessage(m->jsonDocument().toJson());
}

QTcpSocket* MessageEndpoint::socket()
{
	return mSocket;
}

bool MessageEndpoint::operator ==(MessageEndpoint* endpoint)
{
	return endpoint->socket()->peerAddress().toString() == mSocket->peerAddress().toString();
}

bool MessageEndpoint::operator !=(MessageEndpoint* endpoint)
{
	return !(this==endpoint);
}
void MessageEndpoint::handleSocketError(QAbstractSocket::SocketError error)
{
	emit connectionFailed(ConnectionError(mSocket, error));
}

void MessageEndpoint::handleProtocolError(QString error)
{
	error.replace("%r", mSocket->localAddress().toString());

	qDebug() << error << endl;
}
void MessageEndpoint::connected()
{
	mRemoteIdentity.name()->requestResource();
	mRemoteIdentity.picture()->requestResource();
}

