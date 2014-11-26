#include "MessageServer.hpp"

#include <iostream>
#include <stdexcept>
#include <QHostAddress>
#include <QTimer>
#include "ClientManager.hpp"

using namespace std;

MessageServer::MessageServer(ClientManager* clientManager, QTcpSocket* socket, QObject* parent)
	: MessageEndpoint(clientManager, socket, parent)
{
}

void MessageServer::requestIdentification()
{
	cout << "[MessageServer] Requesting identification from client." << endl;
	writeInternalMessageString(IDENTITY_REQUEST_STRING, IDENTIFICATION);
	mIdentState = CLIENT_IDENTITY_REQUESTED;
}

void MessageServer::handleIndentityMessage(ChatMessage* request)
{
	if(mIdentState == NOT_IDENTIFIED)
	{
		writeInternalMessageString(REQUIRES_IDENTIFICATON_STRING, IDENTIFICATION);
		emit unidentifiedClientSentMessage();
	}
	else if(mIdentState == CLIENT_IDENTITY_REQUESTED)
	{
		identityRecieved(request);
	}
	else if(mIdentState == CLIENT_IDENTIFIED)
	{
		if(request->messageDataAsUTF8String() == IDENTITY_REQUEST_STRING)
		{
			mIdentState = SERVER_IDENTITY_REQUESTED;
			sendIdentity();
		}
	}
	else if(mIdentState == SERVER_IDENTITY_SENT)
	{
		if(request->messageDataAsUTF8String() == IDENTIFIED_STRING)
		{
			cout << "[MessageServer] Identification complete." << endl;
			mIdentState = IDENTIFICATION_COMPLETE;
			emit identificationSuccesful();
		}
		else
		{
			cerr << "[MessageServer] An unhandled protocol error occured: " << request->messageDataAsUTF8String().toStdString() << endl;
			mIdentState = CLIENT_IDENTIFIED;
			emit identificationFailed(ConnectionError(IDENT_UNSPECIFIED_ERROR));
		}
	}
}
void MessageServer::identityRecieved(ChatMessage* m)
{
	QString clientName = m->messageDataAsUTF8String();
	if(clientName.isEmpty())
	{
		writeInternalMessageString(SENT_EMPTY_NAME_STRING, IDENTIFICATION);
		cerr << "Remote host " + mSocket->localAddress().toString().toStdString()
							+ " violated IDENTIFY protocol. They sent back an empty name.";
		mIdentState = NOT_IDENTIFIED;
		emit identificationFailed(ConnectionError(IDENT_SENT_EMPTY_NAME));
	}
	else
	{
		writeInternalMessageString(IDENTIFIED_STRING, IDENTIFICATION);
		setRemoteName(clientName);
		mIdentState = CLIENT_IDENTIFIED;

		cout << "[MessageServer] Client successfully identified with name: " << mRemoteName.toStdString() << endl;
	}
}
void MessageServer::sendIdentity()
{
	cout << "[MessageServer] Sending identity to client." << endl;

	writeInternalMessageString(mClientManager->localName(), IDENTIFICATION);
	mIdentState = SERVER_IDENTITY_SENT;
}
void MessageServer::processInternalMessage(ChatMessage* m)
{
//	cout << "[MessageServer] Processing internal message." << endl;

	if(m->flags().type() != IDENTIFICATION)
	{
		handleIndentityMessage(m);
	}
	else
	{
		cout << "[MessageServer] Passing internal message down." << endl;
		//Client has been IDENTIFIED.

		//We don't handle any other messages, pass them down.
		MessageEndpoint::processInternalMessage(m);
	}
}
