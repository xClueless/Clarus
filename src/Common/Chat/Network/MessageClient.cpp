#include "MessageClient.hpp"

#include <iostream>
#include "ClientManager.hpp"

using namespace std;

MessageClient::MessageClient(ClientManager* clientManager, QTcpSocket* socket, QObject* parent)
	: MessageEndpoint(clientManager, socket, parent)
{
}

void MessageClient::connectToServer(QString clientHostname, quint16 mPort)
{
	mSocket->connectToHost(clientHostname, mPort);
}

void MessageClient::sendName()
{
	writeInternalMessageString(mClientManager->localName());
	mIdentState = CLIENT_IDENTITY_SENT;
}

void MessageClient::processInternalMessage(ChatMessage* request)
{
//	cout << "[MessageClient] Processing internal message." << endl;
	if(mIdentState != IDENTIFICATION_COMPLETE)
	{
		handleIdentityMessage(request);
	}
	else
	{
		cout << "[MessageClient] Passing internal message down." << endl;
		//We don't understand this. Pass it down.
		MessageEndpoint::processInternalMessage(request);
	}
//	cout << "[MessageClient] Finished internal message handler" << endl;
}

void MessageClient::handleIdentityMessage(ChatMessage* request)
{
	if(mIdentState == NOT_IDENTIFIED)
	{
		if(request->messageAsUTF8String() == IDENTITY_REQUEST_STRING)
		{
			mIdentState = CLIENT_IDENTITY_REQUESTED;
			sendName();
		}
		else
		{
			cerr << "[MessageClient] Server sent us an unexpected message '" << request->messageAsUTF8String().toStdString()
				 << "' for state " << identStateString().toStdString() << endl;
			writeInternalMessageString("NOT_YET_IDENTIFIED");
		}
	}
	else if(mIdentState == CLIENT_IDENTITY_SENT)
	{
		if(request->messageAsUTF8String() == SENT_EMPTY_NAME_STRING)
		{
			mIdentState = NOT_IDENTIFIED;
			emit identificationFailed(ConnectionError(IDENT_SENT_EMPTY_NAME));
		}
		else if(request->messageAsUTF8String() == IDENTIFIED_STRING)
		{
			cout << "[MessageClient] Server sent us confirmation of our identification." << endl;
			cout << "[MessageClient] Asking server for its identification." << endl;
			writeInternalMessageString(IDENTITY_REQUEST_STRING);
			mIdentState = SERVER_IDENTITY_REQUESTED;
		}
		else
		{
			emit identificationFailed(ConnectionError(IDENT_UNSPECIFIED_ERROR));
			mIdentState = NOT_IDENTIFIED;
		}
	}
	else if(mIdentState == SERVER_IDENTITY_REQUESTED)
	{
		identityRecieved(request);
	}
	else if(mIdentState == IDENTIFICATION_COMPLETE)
	{
		cerr << "[MessageClient] Server sent us an ident message when we have already identified ourself." << endl;
	}
}

void MessageClient::identityRecieved(ChatMessage* m)
{
	QString serverName = m->messageAsUTF8String();
	if(serverName.isEmpty())
	{
		writeInternalMessageString(SENT_EMPTY_NAME_STRING);
		cerr << "Remote host " + mSocket->localAddress().toString().toStdString()
							+ " violated IDENTIFY protocol. They sent back an empty name.";
		mIdentState = NOT_IDENTIFIED;
		emit identificationFailed(ConnectionError(IDENT_SENT_EMPTY_NAME));
	}
	else
	{
		setRemoteName(serverName);
		writeInternalMessageString(IDENTIFIED_STRING);
		mIdentState = IDENTIFICATION_COMPLETE;

		cout << "[MessageClient] Server identified successfully with name: " << mRemoteName.toStdString() << endl;

		emit identificationSuccesful();
	}
}
