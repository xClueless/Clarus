#include "ContactWindow.hpp"

ContactWindow::ContactWindow(MessageMarshaller* marshaller, QWidget *parent) :
	QWidget(parent), mMarshaller(marshaller)
{
	setWindowTitle("Contacts");
	mLayout = new QGridLayout(this);
	setLayout(mLayout);

	mContactButtons.append(new QPushButton("127.0.0.1", this));

	for(QPushButton* contactButton : mContactButtons)
	{
		mLayout->addWidget(contactButton);
		connect(contactButton, SIGNAL(clicked()), this, SLOT(contactButtonClicked()));
	}

}

void ContactWindow::contactButtonClicked()
{
	QPushButton* senderButton = (QPushButton*) sender();

	QString targetContact = senderButton->text();

	if(mMarshaller->clients()[targetContact] == NULL)
	{
		mMarshaller->connectToClient(targetContact);
	}
	MessageClient* targetClient = mMarshaller->clients()[targetContact];

	QList<MessageClient*> targetClients;
	targetClients << targetClient;
	ChatWindow* newChatWindow = new ChatWindow(targetClients);
	mChatWindows.append(newChatWindow);
}
