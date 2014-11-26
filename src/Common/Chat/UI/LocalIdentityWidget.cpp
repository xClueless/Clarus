#include "LocalIdentityWidget.hpp"

#include <QFileDialog>

LocalIdentityWidget::LocalIdentityWidget(ClientManager* clientManager, QWidget* parent)
	: QWidget(parent), mClientManager(clientManager)
{
	mLayout = new QHBoxLayout(this);
	setLayout(mLayout);

	mLocalPixmapButton = new QPushButton(this);
	updatePixmap();
	connect(mLocalPixmapButton, SIGNAL(clicked()), this, SLOT(selectNewPixmap()));
	mLayout->addWidget(mLocalPixmapButton);

	mLocalNameLabel = new QLabel("Name:", this);
	mLayout->addWidget(mLocalNameLabel);

	mLocalNameBox = new QLineEdit(this);
	mLocalNameBox->setText(mClientManager->localName());
	connect(mLocalNameBox, SIGNAL(returnPressed()), this, SLOT(sendNewLocalName()));
	connect(mClientManager, SIGNAL(localNameChanged()), this, SLOT(retrieveNewLocalName()));

	mLayout->addWidget(mLocalNameBox);

	connect(mClientManager, SIGNAL(localPixmapChanged()), this, SLOT(updatePixmap()));
}

void LocalIdentityWidget::sendNewLocalName()
{
	mClientManager->setLocalName(mLocalNameBox->text());
}

void LocalIdentityWidget::retrieveNewLocalName()
{
	mLocalNameBox->setText(mClientManager->localName());
}

void LocalIdentityWidget::selectNewPixmap()
{
	QString pixmapFileName = QFileDialog::getOpenFileName(this, "Select Picture", QString(), "PNG Image (*.png)");
	if(!pixmapFileName.isNull())
	{
		mClientManager->loadLocalPixmap(pixmapFileName);
	}
}

void LocalIdentityWidget::updatePixmap()
{
	mLocalPixmapButton->setIcon(QIcon(mClientManager->localPixmap()));
}
