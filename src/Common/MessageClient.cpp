#include "MessageClient.hpp"

#include <iostream>
#include <stdexcept>
#include <QHostAddress>
#include <QEventLoop>

using namespace std;

MessageClient::MessageClient(QTcpSocket* socket, QObject* parent) : QObject(parent), mSocket(socket), mClientStream(mSocket)
{
	connect(&mClientStream, SIGNAL(messageReady(QString)), this, SLOT(readChatMessage(QString)));
}

void MessageClient::identify(QStringList currentClientNames)
{
	mClientStream.writeMessage("IDENTIFY");

	waitForInternalMessage();
	ChatMessage* clientName = mInternalMessages.dequeue();
	if(clientName->message().isEmpty())
	{
		writeInternalMessage("SENT-EMPTY-NAME");
		throw runtime_error("Remote host " + mSocket->localAddress().toString().toStdString()
							+ " violated IDENTIFY protocol. They sent back an empty name.");
	}
	if(currentClientNames.contains(clientName))
	{
		writeInternalMessage("NAME-TAKEN");
		throw runtime_error("Remote host " + mSocket->localAddress().toString().toStdString()
							+ " tried to use taken name.");
	}

	mRemoteClientName = clientName;
	writeInternalMessage("WELCOME");
}

void MessageClient::sendIdentify()
{
	waitForInternalMessage();
	QString ident = mInternalMessages.dequeue();
	if(ident != "IDENTIFY")
	{
		throw runtime_error("Remote host " + mSocket->localAddress().toString().toStdString()
							+ " violated IDENTIFY protocol. They sent back: " + ident.toStdString());
	}

	writeInternalMessage(mLocalClientName);

	waitForInternalMessage();
	QString welcome = mInternalMessages.dequeue();
	if(welcome != "WELCOME")
	{
		throw runtime_error("[MessageMarshaller] Remote host " + mSocket->localAddress().toString().toStdString()
							+ " violated WELCOME protocol. They sent back: " + welcome.toStdString());
	}
}

void MessageClient::waitForInternalMessage()
{
	if(mInternalMessages.isEmpty())
	{
		QEventLoop messageWaitLoop;
		connect(this, SIGNAL(messageReady(ChatMessage*)), &messageWaitLoop, SLOT(quit()));
		messageWaitLoop.exec();
	}
}

void MessageClient::writeInternalMessage(QString messageString)
{
	ChatMessage internalMessage(MessageFlags(INTERNAL), QStringList(mRemoteClientName), messageString);
	writeChatMessage(&internalMessage);
}

void MessageClient::readChatMessage(QString messageString)
{
	cout << "[MessageClient] Whole message is available. Reading it now." << endl;
	try
	{
		ChatMessage* m = new ChatMessage(messageString, mRemoteClientName);
		if(m->flags() == INTERNAL)
		{
			mInternalMessages.enqueue(m);
		}
		else
		{
			emit messageReady(m);
		}
	}
	catch(const runtime_error& re)
	{
		cerr << "[MessageClient] " << re.what() << endl;
	}
}

void MessageClient::writeChatMessage(ChatMessage* m)
{
	mClientStream.writeMessage(m->messageString());
}

QString MessageClient::remoteClientName()
{
	return mRemoteClientName;
}
