#include "LocalIdentityWidget.hpp"

#include <QFileDialog>

LocalIdentityWidget::LocalIdentityWidget(EndpointManager* endpointManager, QWidget* parent)
	: QWidget(parent), mEndpointManager(endpointManager)
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
	mLocalNameBox->setText(mEndpointManager->localName());
	connect(mLocalNameBox, SIGNAL(returnPressed()), this, SLOT(sendNewLocalName()));
	connect(mEndpointManager, SIGNAL(localNameChanged()), this, SLOT(retrieveNewLocalName()));

	mLayout->addWidget(mLocalNameBox);

	connect(mEndpointManager, SIGNAL(localPixmapChanged()), this, SLOT(updatePixmap()));
}

void LocalIdentityWidget::sendNewLocalName()
{
	mEndpointManager->setLocalName(mLocalNameBox->text());
}

void LocalIdentityWidget::retrieveNewLocalName()
{
	mLocalNameBox->setText(mEndpointManager->localName());
}

void LocalIdentityWidget::selectNewPixmap()
{
	QString pixmapFileName = QFileDialog::getOpenFileName(this, "Select Picture", QString(), "PNG Image (*.png)");
	if(!pixmapFileName.isNull())
	{
		mEndpointManager->loadLocalPixmap(pixmapFileName);
	}
}

void LocalIdentityWidget::updatePixmap()
{
	mLocalPixmapButton->setIcon(QIcon(mEndpointManager->localPixmap()));
}
