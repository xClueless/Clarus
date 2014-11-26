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
	connect(mSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(handleSocketError(QAbstractSocket::SocketError)));
	connect(mClientManager, SIGNAL(localNameChanged()), this, SLOT(notifyRemoteAboutIdentityUpdate()));
	connect(mClientManager, SIGNAL(localPixmapChanged()), this, SLOT(notifyRemoteAboutPixmapUpdate()));
}

MessageEndpoint::~MessageEndpoint()
{
	mSocket->close();
	delete mSocket;
}

void MessageEndpoint::sendIdentError()
{
	writeInternalMessageString("NOT_IDENTIFIED", IDENTIFICATION);
}
void MessageEndpoint::writeInternalMessageString(QString messageString, MessageType type)
{
	writeInternalMessageBytes(messageString.toUtf8(), type, UTF8);
}

void MessageEndpoint::writeInternalMessageBytes(QByteArray messageBytes, MessageType type, MessageFormat format)
{
	ChatMessage internalMessage(MessageFlags(type, format), messageBytes);
	writeChatMessage(&internalMessage);
}

QString MessageEndpoint::identityStateString()
{
	QString identityStateString = "Local: ";
	switch(mLocalIdentState)
	{
		case NOT_IDENTIFIED: identityStateString += "NOT_IDENTIFIED"; break;
		case IDENTITY_REQUESTED: identityStateString += "IDENTITY_REQUESTED"; break;
		case IDENTITY_SENT: identityStateString += "IDENTITY_SENT"; break;
		case IDENTIFIED: identityStateString += "IDENTIFIED"; break;
		default: identityStateString += "UNKNOWN_IDENTITY_STATE";
	}
	identityStateString += " Remote: ";
	switch(mRemoteIdentState)
	{
		case NOT_IDENTIFIED: identityStateString += "NOT_IDENTIFIED"; break;
		case IDENTITY_REQUESTED: identityStateString += "IDENTITY_REQUESTED"; break;
		case IDENTITY_SENT: identityStateString += "IDENTITY_SENT"; break;
		case IDENTIFIED: identityStateString += "IDENTIFIED"; break;
		default: identityStateString += "UNKNOWN_IDENTITY_STATE";
	}
	return identityStateString;
}
QString MessageEndpoint::pixmapStateString()
{
	QString pixmapStateString = "Local: ";
	switch(mLocalPixmapState)
	{
		case PIXMAP_NOT_SENT: pixmapStateString += "PIXMAP_NOT_SENT"; break;
		case PIXMAP_REQUESTED: pixmapStateString += "PIXMAP_REQUESTED"; break;
		case PIXMAP_RECIEVED: pixmapStateString += "PIXMAP_RECIEVED"; break;
		case PIXMAP_UPDATE_AVAILABLE: pixmapStateString += "PIXMAP_UPDATE_AVAILABLE"; break;
		default: pixmapStateString += "UNKNOWN_PIXMAP_STATE";
	}
	pixmapStateString += " Remote: ";
	switch(mRemotePixmapState)
	{
		case PIXMAP_NOT_SENT: pixmapStateString += "PIXMAP_NOT_SENT"; break;
		case PIXMAP_REQUESTED: pixmapStateString += "PIXMAP_REQUESTED"; break;
		case PIXMAP_RECIEVED: pixmapStateString += "PIXMAP_RECIEVED"; break;
		case PIXMAP_UPDATE_AVAILABLE: pixmapStateString += "PIXMAP_UPDATE_AVAILABLE"; break;
		default: pixmapStateString += "UNKNOWN_PIXMAP_STATE";
	}
	return pixmapStateString;
}

void MessageEndpoint::readChatMessage(QByteArray messageBytes)
{
//	cout << "[MessageEndpoint] Whole message is available. Reading it now." << endl;
	try
	{
		ChatMessage* m = new ChatMessage(messageBytes, mRemoteName);
//		cout << "[MessageEndpoint] Message: " << m->messageString().toStdString() << endl;
		if(m->flags().type() != PRIVATE)
		{
			handleInternalMessage(m);
		}
		else
		{
			if(mLocalIdentState != IDENTIFIED || mRemoteIdentState != IDENTIFIED)
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

void MessageEndpoint::setRemoteName(QString name)
{
	mRemoteName = name;
	emit remoteNameChanged();
}

void MessageEndpoint::handleSocketError(QAbstractSocket::SocketError error)
{
	emit connectionFailed(ConnectionError(mSocket, error));
}

void MessageEndpoint::handleInternalMessage(ChatMessage* m)
{
	cout << "[MessageEndpoint] Processing internal message: " << m->messageDataAsUTF8String().toStdString() << endl;

	if(m->flags().type() == IDENTIFICATION)
	{
		handleIndentityMessage(m);
	}
	else if(mLocalIdentState != IDENTIFIED || mRemoteIdentState != IDENTIFIED)
	{
		writeInternalMessageString(REQUIRES_IDENTIFICATON_STRING, IDENTIFICATION);
		emit unidentifiedClientSentMessage();
	}
	else if(m->flags().type() == ENDPOINT_PIXMAP_EXCHANGE)
	{
		handlePixmapExchangeMessage(m);
	}
	else
	{
		cerr << "[MessageEndpoint] Internal message not understood. We don't know how to handle the message type." << endl;
		writeInternalMessageString("UNKNOWN_COMMAND", PROTOCOL_ERROR);
	}
}

void MessageEndpoint::handleIndentityMessage(ChatMessage* request)
{
	QString requestString = request->messageDataAsUTF8String();

	if(mRemoteIdentState == IDENTIFIED && requestString == IDENTITY_UPDATE_AVAILABLE_STRING)
	{
		requestIdentity();
	}
	if(mRemoteIdentState == IDENTITY_REQUESTED)
	{
		identityRecieved(requestString);
	}
	else if(mLocalIdentState == NOT_IDENTIFIED && requestString == IDENTITY_REQUEST_STRING)
	{
		mLocalIdentState = IDENTITY_REQUESTED;
		sendIdentity();
	}
	else if(mLocalIdentState == IDENTITY_SENT)
	{
		if(requestString == IDENTIFIED_STRING)
		{
			cout << "[MessageEndpoint] Remote successfully identified us." << endl;
			mLocalIdentState = IDENTIFIED;
			emit remoteIdentifiedUs();
		}
		else
		{
			cerr << "[MessageEndpoint] An unhandled protocol error occured: " << request->messageDataAsUTF8String().toStdString() << endl;
			mLocalIdentState = NOT_IDENTIFIED;
			emit identificationFailed(ConnectionError(IDENT_UNSPECIFIED_ERROR));
		}
	}
	else
	{
		cerr << "[MessageEndpoint] Identity message: " << requestString.toStdString()
			 << " not understood in current state: " << identityStateString().toStdString() << endl;
		writeInternalMessageString("UNKNOWN_COMMAND", PROTOCOL_ERROR);
	}

	if(mLocalIdentState == IDENTIFIED && mRemoteIdentState == IDENTIFIED)
	{
		cout << "[MessageEndpoint] Identification complete." << endl;
		emit identificationSuccessful();
	}
}
void MessageEndpoint::requestIdentity()
{
	cout << "[MessageEndpoint] Requesting identification from remote." << endl;
	writeInternalMessageString(IDENTITY_REQUEST_STRING, IDENTIFICATION);
	mRemoteIdentState = IDENTITY_REQUESTED;
}

void MessageEndpoint::notifyRemoteAboutIdentityUpdate()
{
	writeInternalMessageString(IDENTITY_UPDATE_AVAILABLE_STRING, IDENTIFICATION);
}
void MessageEndpoint::sendIdentity()
{
	cout << "[MessageEndpoint] Sending identity to remote." << endl;
	writeInternalMessageString(mClientManager->localName(), IDENTIFICATION);
	mLocalIdentState = IDENTITY_SENT;
}
void MessageEndpoint::identityRecieved(QString remoteName)
{
	if(remoteName.isEmpty())
	{
		writeInternalMessageString(SENT_EMPTY_NAME_STRING, IDENTIFICATION);
		cerr << "Remote host " + mSocket->localAddress().toString().toStdString()
							+ " violated IDENTIFY protocol. They sent back an empty name.";
		mRemoteIdentState = NOT_IDENTIFIED;
		emit identificationFailed(ConnectionError(IDENT_SENT_EMPTY_NAME));
	}
	else
	{
		writeInternalMessageString(IDENTIFIED_STRING, IDENTIFICATION);
		setRemoteName(remoteName);
		mRemoteIdentState = IDENTIFIED;
		cout << "[MessageEndpoint] Remote successfully identified with name: " << mRemoteName.toStdString() << endl;
		emit remoteIdentified();
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
		cout << "[MessageEndpoint] Remote has recieved our pixmap." << endl;
		mLocalPixmapState = PIXMAP_RECIEVED;
	}
	else if(mRemotePixmapState == PIXMAP_REQUESTED && pixmapMessage->flags().format() == RAW)
	{
		//Us: SEND_PIXMAP
		//Remote: ${RAWPIXMAP}
		recievePixmap(pixmapMessage);
	}
	else if(mRemotePixmapState == PIXMAP_RECIEVED && messageUTF8 == PIXMAP_UPDATE_AVAILABLE_STRING)
	{
		cout << "[MessageEndpoint] Remote has informed us about an available pixmap update." << endl;
		mRemotePixmapState = PIXMAP_UPDATE_AVAILABLE;
		requestPixmap();
	}
	else
	{
		cerr << "[MessageEndpoint] Pixmap message: " << messageUTF8.toStdString()
			 << " not understood in current state: " << pixmapStateString().toStdString() << endl;
	}
}

void MessageEndpoint::requestPixmap()
{
	cout << "[MessageEndpoint] Requesting remote pixmap." << endl;
	writeInternalMessageString(PIXMAP_REQUEST_STRING, ENDPOINT_PIXMAP_EXCHANGE);
	mRemotePixmapState = PIXMAP_REQUESTED;
}

void MessageEndpoint::sendPixmap()
{
	cout << "[MessageEndpoint] Sending pixmap to remote." << endl;
	QByteArray pixmapArray;
	QBuffer pixmapBuffer(&pixmapArray);
	pixmapBuffer.open(QIODevice::WriteOnly);
	mClientManager->localPixmap().save(&pixmapBuffer, "PNG");
	writeInternalMessageBytes(pixmapArray, ENDPOINT_PIXMAP_EXCHANGE, RAW);
	mLocalPixmapState = PIXMAP_SENT;
}

void MessageEndpoint::notifyRemoteAboutPixmapUpdate()
{
	cout << "[MessageEndpoint] Notifying remote about available pixmap update." << endl;
	mLocalPixmapState = PIXMAP_UPDATE_AVAILABLE;
	writeInternalMessageString(PIXMAP_UPDATE_AVAILABLE_STRING, ENDPOINT_PIXMAP_EXCHANGE);
}

void MessageEndpoint::recievePixmap(ChatMessage* m)
{
	cout << "[MessageEndpoint] Recieved pixmap from remote." << endl;

	if(mRemotePixmap.loadFromData(m->messageData(), "PNG"))
	{
		mRemotePixmapState = PIXMAP_RECIEVED;
		writeInternalMessageString(PIXMAP_RECIEVED_STRING, ENDPOINT_PIXMAP_EXCHANGE);
		emit remotePixmapChanged();
	}
	else
	{
		cerr << "[MessageEndpoint] Failed to load remote pixmap." << endl;
		mRemotePixmapState = PIXMAP_NOT_SENT;
	}
}

