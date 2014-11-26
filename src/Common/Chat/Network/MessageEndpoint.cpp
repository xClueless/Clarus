#include "MessageEndpoint.hpp"

#include <iostream>
#include <QHostAddress>
#include <QBuffer>
#include "ClientManager.hpp"

using namespace std;

MessageEndpoint::MessageEndpoint(ClientManager* clientManager, QTcpSocket* socket, QObject *parent) :
	QObject(parent), mClientManager(clientManager), mSocket(socket), mNetworkStream(mSocket)
{
	connect(&mNetworkStream, SIGNAL(messageReady(QByteArray)), this, SLOT(readChatMessage(QByteArray)));
	connect(this, SIGNAL(internalMessageReady(ChatMessage*)), this, SLOT(processInternalMessage(ChatMessage*)));
	connect(mSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(handleSocketError(QAbstractSocket::SocketError)));
	connect(mClientManager, SIGNAL(localPixmapChanged()), this, SLOT(notifyRemoteAboutPixmapUpdate()));
}

MessageEndpoint::~MessageEndpoint()
{
	mSocket->close();
	delete mSocket;
}

void MessageEndpoint::sendIdentError()
{
	writeInternalMessageString("NOT_IDENTIFIED");
}
void MessageEndpoint::writeInternalMessageString(QString messageString)
{
	writeInternalMessageBytes(messageString.toUtf8(), UTF8);
}

void MessageEndpoint::writeInternalMessageBytes(QByteArray messageBytes, MessageFormat format)
{
	ChatMessage internalMessage(MessageFlags(INTERNAL, format), messageBytes);
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

void MessageEndpoint::readChatMessage(QByteArray messageBytes)
{
//	cout << "[MessageEndpoint] Whole message is available. Reading it now." << endl;
	try
	{
		ChatMessage* m = new ChatMessage(messageBytes, mRemoteName);
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
				cout << "[MessageEndpoint] Alerting subscribers to new message: " << m->messageDataAsUTF8String().toStdString() << endl;
				emit messageReady(m);
			}
		}
	}
	catch(const runtime_error& re)
	{
		cerr << "[MessageEndpoint::readChatMessage(QByteArray)] Error: " << re.what() << endl;
	}
}

void MessageEndpoint::writeChatMessage(ChatMessage* m)
{
	cout << "[MessageEndpoint] Writing message '" << m->messageDataAsUTF8String().toStdString() << "' with flags: " << m->flags().flagString().toStdString() << endl;
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

void MessageEndpoint::processInternalMessage(ChatMessage* m)
{
	cout << "[MessageEndpoint] Processing internal message: " << m->messageDataAsUTF8String().toStdString() << endl;

	if((mLocalPixmapState == PIXMAP_NOT_SENT || mLocalPixmapState == PIXMAP_UPDATE_AVAILABLE)
			&& m->messageDataAsUTF8String() == PIXMAP_REQUEST_STRING)
	{
		sendPixmap();
	}
	else if(mLocalPixmapState == PIXMAP_SENT && m->messageDataAsUTF8String() == PIXMAP_RECIEVED_STRING)
	{
		cout << "[MessageEndpoint] Remote has recieved our pixmap." << endl;
		mLocalPixmapState = PIXMAP_RECIEVED;
	}
	else if(mRemotePixmapState == PIXMAP_REQUESTED && m->flags().type() == RAW)
	{
		//Us: SEND_PIXMAP
		//Remote: ${RAWPIXMAP}
		recievePixmap(m);
	}
	else if(mRemotePixmapState == PIXMAP_SENT && m->messageDataAsUTF8String() == PIXMAP_UPDATE_AVAILABLE_STRING)
	{
		cout << "[MessageEndpoint] Remote has informed us about an available pixmap update." << endl;
		mRemotePixmapState = PIXMAP_UPDATE_AVAILABLE;
		requestPixmap();
	}
	else
	{
		cerr << "[MessageEndpoint] Internal message: " << m->messageDataAsUTF8String().toStdString()
			 << " not understood in current state: " << identStateString().toStdString() << endl;
		writeInternalMessageString("UNKNOWN_COMMAND");
		while(true)
		{

		}
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

void MessageEndpoint::requestPixmap()
{
	cout << "[MessageEndpoint] Requesting remote pixmap." << endl;
	writeInternalMessageString(PIXMAP_REQUEST_STRING);
	mRemotePixmapState = PIXMAP_REQUESTED;
}

void MessageEndpoint::sendPixmap()
{
	cout << "[MessageEndpoint] Sending pixmap to remote." << endl;
	QByteArray pixmapArray;
	QBuffer pixmapBuffer(&pixmapArray);
	pixmapBuffer.open(QIODevice::WriteOnly);
	mClientManager->localPixmap().save(&pixmapBuffer, "PNG");
	writeInternalMessageBytes(pixmapArray, RAW);
	mLocalPixmapState = PIXMAP_SENT;
}

void MessageEndpoint::notifyRemoteAboutPixmapUpdate()
{
	cout << "[MessageEndpoint] Notifying remote about available pixmap update." << endl;
	mLocalPixmapState = PIXMAP_UPDATE_AVAILABLE;
	writeInternalMessageString(PIXMAP_UPDATE_AVAILABLE_STRING);
}

void MessageEndpoint::recievePixmap(ChatMessage* m)
{
	cout << "[MessageEndpoint] Recieved pixmap from remote." << endl;

	if(mRemotePixmap.loadFromData(m->messageData(), "PNG"))
	{
		mRemotePixmapState = PIXMAP_RECIEVED;
		writeInternalMessageString(PIXMAP_RECIEVED_STRING);
		emit remotePixmapChanged();
	}
	else
	{
		cerr << "[MessageEndpoint] Failed to load remote pixmap." << endl;
		mRemotePixmapState = PIXMAP_NOT_SENT;
	}
}

