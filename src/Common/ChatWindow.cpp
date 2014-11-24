#include "ChatWindow.hpp"

ChatWindow::ChatWindow(QList<MessageClient*> conversationClients, QWidget *parent) :
	QWidget(parent)
{
	mLayout = new QVBoxLayout(this);
	this->setLayout(mLayout);

	mMessageArea = new QTextEdit();
	mInputBox = new QLineEdit();

	mLayout->addWidget(mMessageArea);
	mLayout->addWidget(mInputBox);

	connect(mInputBox, SIGNAL(returnPressed()), this, SLOT(messageReadyToSend()));

	for(MessageClient* client : conversationClients)
	{
		addClient(client);
	}
}

QStringList ChatWindow::clientStringList()
{
	QStringList clientNames;
	for(MessageClient* mc : mConversationClients)
	{
		clientNames << mc->remoteClientName();
	}
	return clientNames;
}

void ChatWindow::messageReadyToSend()
{
	mConversationClients.at(0);
	ChatMessage* m = new ChatMessage(MessageFlags(PRIVATE), clientStringList(), mInputBox->text());
	emit sendMessage(m);
	delete m;

	mInputBox->clear();
}

void ChatWindow::displayMessage(ChatMessage* m)
{
	mMessageArea->append(m->sender() + ":" + m->message());
}

void ChatWindow::addClient(MessageClient* mc)
{
	connect(mc, SIGNAL(messageReady(ChatMessage*)), this, SLOT(displayMessage(ChatMessage*)));
//	mConversationClients.append(mc);
	setWindowTitle(clientStringList().join(", "));
}
