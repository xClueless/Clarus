#include "EndpointListWidget.hpp"

EndpointListWidget::EndpointListWidget(ClientManager* clientManager, QWidget *parent) :
	QWidget(parent), mClientManager(clientManager)
{
	setWindowTitle("Contacts");

	mLayout = new QGridLayout(this);
	setLayout(mLayout);

	connect(mClientManager, SIGNAL(endpointIdentified(MessageEndpoint*)), this, SLOT(endpointIdentified(MessageEndpoint*)));
}

EndpointListWidget::~EndpointListWidget()
{
	for(ChatWindow* chatWindow : mChatWindows)
	{
		delete chatWindow;
	}
}

void EndpointListWidget::endpointButtonClicked()
{
	EndpointButton* senderButton = (EndpointButton*) sender();
	openConversation(senderButton->group());
}

void EndpointListWidget::endpointIdentified(MessageEndpoint* endpoint)
{
	ChatGroup* group = new ChatGroup(mClientManager, endpoint, this);
	mChatGroups << group;
	EndpointButton* newEndpointButton = new EndpointButton(group, this);
	connect(newEndpointButton, SIGNAL(clicked()), this, SLOT(endpointButtonClicked()));
	connect(group, SIGNAL(messageReady(ChatMessage*)), this, SLOT(openNewConversation(ChatMessage*)));
	mEndpointButtons << newEndpointButton;
	mLayout->addWidget(newEndpointButton);
}

void EndpointListWidget::openConversation(ChatGroup* group)
{
	disconnect(group, SIGNAL(messageReady(ChatMessage*)), this, SLOT(openNewConversation(ChatMessage*)));
	ChatWindow* newChatWindow = new ChatWindow(group);
	mChatWindows << newChatWindow;
	newChatWindow->show();
}

void EndpointListWidget::openNewConversation(ChatMessage* message)
{
	ChatGroup* group = (ChatGroup*) sender();
	openConversation(group);
	mChatWindows.back()->displayMessage(message);
}
