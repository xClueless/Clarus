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

	mIdentityExchange = new IdentityExchange(this);
	connect(mIdentityExchange, SIGNAL(protocolError(QString)), this, SLOT(handleProtocolError(QString)));
	connect(mIdentityExchange, SIGNAL(sendMessage(ChatMessage*)), this, SLOT(writeChatMessage(ChatMessage*)));
	connect(mEndpointManager, SIGNAL(localNameChanged()), mIdentityExchange, SLOT(notifyRemoteAboutIdentityUpdate()));
}

MessageEndpoint::~MessageEndpoint()
{
	mSocket->close();
	delete mSocket;
}

void MessageEndpoint::sendIdentError()
{
	writeInternalMessageString("NOT_IDENTIFIED", MessageType::IDENTIFICATION);
}
void MessageEndpoint::writeInternalMessageString(QString messageString, MessageType type)
{
	writeInternalMessageBytes(messageString.toUtf8(), type, MessageFormat::UTF8);
}

void MessageEndpoint::writeInternalMessageBytes(QByteArray messageBytes, MessageType type, MessageFormat format)
{
	ChatMessage internalMessage(MessageFlags(type, format), messageBytes);
	writeChatMessage(&internalMessage);
}

void MessageEndpoint::readChatMessage(QByteArray messageBytes)
{
//	cout << "[MessageEndpoint] Whole message is available. Reading it now." << endl;
	try
	{
		ChatMessage* m = new ChatMessage(messageBytes, mRemoteName);
//		cout << "[MessageEndpoint] Message: " << m->messageString().toStdString() << endl;
		if(m->flags().type() != MessageType::PRIVATE)
		{
			handleInternalMessage(m);
		}
		else
		{
			if(!mIdentityExchange->identificationIsComplete())
			{
				sendIdentError();
			}
			else
			{
				qDebug() << "[MessageEndpoint] Alerting subscribers to new message: " << m->messageDataAsUTF8String() << endl;
				emit messageReady(m);
			}
		}
	}
	catch(const runtime_error& re)
	{
		qDebug() << "[MessageEndpoint::readChatMessage(QByteArray)] Error: " << re.what() << endl;
	}
}

void MessageEndpoint::writeChatMessage(ChatMessage* m)
{
	qDebug() << "[MessageEndpoint] Writing message '" << m->messageDataAsUTF8String() << "' with flags: " << m->flags().flagString() << endl;
	mNetworkStream.writeMessage(m->rawMessageBytes());
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

void MessageEndpoint::handleInternalMessage(ChatMessage* m)
{
	qDebug() << "[MessageEndpoint] Processing internal message: " << m->messageDataAsUTF8String() << endl;

	if(m->flags().type() == MessageType::IDENTIFICATION)
	{
		mIdentityExchange->process(m);
	}
	else if(!mIdentityExchange->identificationIsComplete())
	{
		mIdentityExchange->sendIdentityRequiredError();
	}
	else if(m->flags().type() == MessageType::PIXMAP_EXCHANGE)
	{
		handlePixmapExchangeMessage(m);
	}
	else
	{
		qDebug() << "[MessageEndpoint] Internal message not understood. We don't know how to handle the message type." << endl;
		writeInternalMessageString("UNKNOWN_COMMAND", MessageType::PROTOCOL_ERROR);
	}
}

void MessageEndpoint::handlePixmapExchangeMessage(ChatMessage* pixmapMessage)
{
	QString messageUTF8 = pixmapMessage->messageDataAsUTF8String();
	if((mLocalPixmapState == PIXMAP_NOT_SENT || mLocalPixmapState == PIXMAP_UPDATE_AVAILABLE)
			&& messageUTF8 == PIXMAP_REQUEST_STRING)
	{
		sendPixmap();
	}
	else if(mLocalPixmapState == PIXMAP_SENT && messageUTF8 == PIXMAP_RECIEVED_STRING)
	{
		qDebug() << "[MessageEndpoint] Remote has recieved our pixmap." << endl;
		mLocalPixmapState = PIXMAP_RECIEVED;
	}
	else if(mRemotePixmapState == PIXMAP_REQUESTED && pixmapMessage->flags().format() == MessageFormat::RAW)
	{
		//Us: SEND_PIXMAP
		//Remote: ${RAWPIXMAP}
		recievePixmap(pixmapMessage);
	}
	else if(mRemotePixmapState == PIXMAP_RECIEVED && messageUTF8 == PIXMAP_UPDATE_AVAILABLE_STRING)
	{
		qDebug() << "[MessageEndpoint] Remote has informed us about an available pixmap update." << endl;
		mRemotePixmapState = PIXMAP_UPDATE_AVAILABLE;
		requestPixmap();
	}
	else
	{
		qDebug() << "[MessageEndpoint] Pixmap message: " << messageUTF8
			 << " not understood in current state: " << pixmapStateString() << endl;
	}
}

void MessageEndpoint::notifyRemoteAboutPixmapUpdate()
{
	qDebug() << "[MessageEndpoint] Notifying remote about available pixmap update." << endl;
	mLocalPixmapState = PIXMAP_UPDATE_AVAILABLE;
	writeInternalMessageString(PIXMAP_UPDATE_AVAILABLE_STRING, MessageType::PIXMAP_EXCHANGE);
}

void MessageEndpoint::recievePixmap(ChatMessage* m)
{
	qDebug() << "[MessageEndpoint] Recieved pixmap from remote." << endl;

	if(mRemotePixmap.loadFromData(m->messageData(), "PNG"))
	{
		mRemotePixmapState = PIXMAP_RECIEVED;
		writeInternalMessageString(PIXMAP_RECIEVED_STRING, MessageType::PIXMAP_EXCHANGE);
		emit remotePixmapChanged();
	}
	else
	{
		qDebug() << "[MessageEndpoint] Failed to load remote pixmap." << endl;
		mRemotePixmapState = PIXMAP_NOT_SENT;
	}
}

