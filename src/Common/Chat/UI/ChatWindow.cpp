#include "ChatWindow.hpp"

#include <iostream>
#include <QApplication>

using namespace std;

ChatWindow::ChatWindow(ChatGroup* group, QWidget *parent) :
	QWidget(parent), mGroup(group)
{
	mLayout = new QVBoxLayout(this);
	this->setLayout(mLayout);

	mGroupIdentityWidget = new GroupHeaderWidget(mGroup, this);
	mLayout->addWidget(mGroupIdentityWidget);

//	mMessageModel.setStringList(mMessages);
//	mConversationView.setModel(&mMessageModel);
//	mLayout->addWidget(&mConversationView);

	mLayout->addWidget(&mMessageListWidget);

	mInputBox = new QLineEdit();
	mLayout->addWidget(mInputBox);
	connect(mInputBox, SIGNAL(returnPressed()), this, SLOT(messageReadyToSend()));
	connect(mGroup, SIGNAL(messageReady(ChatMessage*)), this, SLOT(displayMessage(ChatMessage*)));

	setMinimumSize(400, 400);
}

ChatGroup* ChatWindow::group()
{
	return mGroup;
}

void ChatWindow::messageReadyToSend()
{
	ChatMessage message(MessageFlags(MessageType::PRIVATE, MessageFormat::UTF8), mInputBox->text());
	mGroup->messageAll(&message);
	addMessageBox("Me:" + message.messageDataAsUTF8String());
	mInputBox->clear();
}

void ChatWindow::displayMessage(ChatMessage* m)
{
	addMessageBox(m->sender() + ":" + m->messageDataAsUTF8String());

	QApplication::alert(this, 300);
//	mMessageModel.setStringList(mMessages);
}
void ChatWindow::addMessageBox(QString messageDisplayString)
{
//	mMessages << s;
//	mMessageModel.setStringList(mMessages);
	QListWidgetItem* listWidgetItem = new QListWidgetItem("");

	QString sender = messageDisplayString.section(':', 0, 0);
	QString messageString = messageDisplayString.section(':', 1);

	cerr << "[ChatWindow] Probable memory leak in addMessageBox(QString)" << endl;
	ChatMessage* newMessage = new ChatMessage(
				MessageFlags(MessageType::PRIVATE, MessageFormat::UTF8),
				messageString);

	newMessage->setSender(sender);
	MessageLabel* widget = new MessageLabel(newMessage);

	mMessageListWidget.addItem(listWidgetItem);
	mMessageListWidget.setItemWidget(listWidgetItem, widget);

	mMessageListWidget.scrollToBottom();
}
