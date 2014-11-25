#include "ContactWindow.hpp"

ContactWindow::ContactWindow(ClientManager* clientManager, QWidget *parent) :
	QWidget(parent), mClientManager(clientManager)
{
	setWindowTitle("Contacts");
	mLayout = new QGridLayout(this);
	setLayout(mLayout);

	this->setMinimumSize(400, 600);
}

void ContactWindow::endpointButtonClicked()
{
	EndpointButton* senderButton = (EndpointButton*) sender();
	openConversation(senderButton->group());
}

void ContactWindow::endpointIdentified(MessageEndpoint* endpoint)
{
	ChatGroup* group = new ChatGroup(mClientManager, endpoint);
	mChatGroups << group;
	EndpointButton* newEndpointButton = new EndpointButton(group);
	connect(newEndpointButton, SIGNAL(clicked()), this, SLOT(endpointButtonClicked()));
	connect(group, SIGNAL(messageReady(ChatMessage*)), this, SLOT(openNewConversation(ChatMessage*)));
	mEndpointButtons << newEndpointButton;
	mLayout->addWidget(newEndpointButton);
}

void ContactWindow::openConversation(ChatGroup* group)
{
	disconnect(group, SIGNAL(messageReady(ChatMessage*)), this, SLOT(openNewConversation(ChatMessage*)));
	ChatWindow* newChatWindow = new ChatWindow(group);
	mChatWindows << newChatWindow;
	newChatWindow->show();
}

void ContactWindow::openNewConversation(ChatMessage* message)
{
	ChatGroup* group = (ChatGroup*) sender();
	openConversation(group);
	mChatWindows.back()->displayMessage(message);
}
