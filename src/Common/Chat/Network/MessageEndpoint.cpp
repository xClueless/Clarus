#include "MessageEndpoint.hpp"

#include <QHostAddress>
#include <QBuffer>
#include <QDebug>
#include "EndpointManager.hpp"

using namespace std;

MessageEndpoint::MessageEndpoint(EndpointManager* endpointManager, QTcpSocket* socket, QObject *parent) :
	QObject(parent), mEndpointManager(endpointManager), mSocket(socket), mNetworkStream(mSocket)
{
	connect(&mNetworkStream, SIGNAL(messageReady(QByteArray)), this, SLOT(readChatMessage(QByteArray)));
	connect(mSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(handleSocketError(QAbstractSocket::SocketError)));
	connect(mEndpointManager, SIGNAL(localPixmapChanged()), this, SLOT(notifyRemoteAboutPixmapUpdate()));


	mRemoteIdentity = new RemoteResource("EndpointName", this);

	connect(mRemoteIdentity, SIGNAL(writeChatMessage(ChatMessage*)), this, SLOT(writeChatMessage(ChatMessage*)));
}

MessageEndpoint::~MessageEndpoint()
{
	mSocket->close();
	delete mSocket;
}

void MessageEndpoint::readChatMessage(QByteArray messageBytes)
{
//	cout << "[MessageEndpoint] Whole message is available. Reading it now." << endl;
	try
	{
		QJsonDocument json = QJsonDocument::fromJson(messageBytes);
		ChatMessage* m = new ChatMessage(json, "");
//		cout << "[MessageEndpoint] Message: " << m->messageString().toStdString() << endl;
		if(m->type() == ChatMessage::MessageType::RESOURCE_EXCHANGE)
		{
			if(m->sections().contains("ResourceName"))
			{
				mSharedResources[m->section("ResourceName").toString()]->handleMessage(m);
			}
			else
			{
				qDebug() << "[MessageEndpoint] Error. Resource is not registered.";
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
	qDebug() << "[MessageEndpoint] Writing message";
	mNetworkStream.writeMessage(m->jsonDocument().toJson());
}

QString MessageEndpoint::remoteName()
{
	return mRemoteName;
}

QTcpSocket* MessageEndpoint::socket()
{
	return mSocket;
}

QPixmap& MessageEndpoint::remotePixmap()
{
	return mRemotePixmap;
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
