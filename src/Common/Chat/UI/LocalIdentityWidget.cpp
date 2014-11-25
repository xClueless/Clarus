#include "LocalIdentityWidget.hpp"

LocalIdentityWidget::LocalIdentityWidget(ClientManager* clientManager, QWidget* parent)
	: QWidget(parent), mClientManager(clientManager)
{
	mLayout = new QHBoxLayout(this);
	setLayout(mLayout);

	mLocalNameLabel = new QLabel("Name:", this);
	mLayout->addWidget(mLocalNameLabel);

	mLocalNameBox = new QLineEdit(this);
	mLocalNameBox->setText(mClientManager->localName());
	connect(mLocalNameBox, SIGNAL(returnPressed()), this, SLOT(sendNewLocalName()));
	connect(mClientManager, SIGNAL(localNameChanged()), this, SLOT(retrieveNewLocalName()));

	mLayout->addWidget(mLocalNameBox);
}

void LocalIdentityWidget::sendNewLocalName()
{
	mClientManager->setLocalName(mLocalNameBox->text());
}

void LocalIdentityWidget::retrieveNewLocalName()
{
	mLocalNameBox->setText(mClientManager->localName());
}
