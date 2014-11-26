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
	ChatMessage internalMessage(INTERNAL, messageString);
	writeChatMessage(&internalMessage);
}

void MessageEndpoint::writeInternalMessageBytes(QByteArray messageBytes)
{
	ChatMessage internalMessage(INTERNAL, messageBytes);
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
				cout << "[MessageEndpoint] Alerting subscribers to new message: " << m->messageAsUTF8String().toStdString() << endl;
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
	cout << "[MessageEndpoint] Writing message with flags: " << m->flags().flagString().toStdString() << endl;
	mNetworkStream.writeMessage(m->messageBytes());
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
	if(m->messageAsUTF8String() == PIXMAP_REQUEST_STRING)
	{
		sendPixmap();
	}
	else if(mLocalPixmapState == PIXMAP_SENT && m->messageAsUTF8String() == PIXMAP_RECIEVED_STRING)
	{
		mLocalPixmapState = PIXMAP_RECIEVED;
	}
	else if(mRemotePixmapState == PIXMAP_REQUESTED)
	{
		recievePixmap(m);
	}
	else
	{
		cerr << "[MessageEndpoint] Internal message: " << m->messageAsUTF8String().toStdString()
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
	writeInternalMessageBytes(pixmapArray);
	mLocalPixmapState = PIXMAP_SENT;
}

void MessageEndpoint::recievePixmap(ChatMessage* m)
{
	cout << "[MessageEndpoint] Recieving pixmap to remote." << endl;

	if(mRemotePixmap.loadFromData(m->messageBytes(), "PNG"))
	{
		mRemotePixmapState = PIXMAP_RECIEVED;
		writeInternalMessageString(PIXMAP_RECIEVED_STRING);
	}
	m->messageBytes();
}

