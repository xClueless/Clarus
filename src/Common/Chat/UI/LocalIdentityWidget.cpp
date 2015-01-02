#include "LocalIdentityWidget.hpp"

#include <QFileDialog>

LocalIdentityWidget::LocalIdentityWidget(LocalIdentity* localIdentity, QWidget* parent)
	: QWidget(parent), mLocalIdentity(localIdentity)
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
	mLocalNameBox->setText(mLocalIdentity->name()->string());
	connect(mLocalNameBox, SIGNAL(returnPressed()), this, SLOT(setLocalName()));

	connect(mLocalIdentity->name(), SIGNAL(dataChanged()), this, SLOT(retrieveNewLocalName()));
	connect(mLocalIdentity->picture(), SIGNAL(dataChanged()), this, SLOT(updatePixmap()));

	mLayout->addWidget(mLocalNameBox);

}

void LocalIdentityWidget::setLocalName()
{
	mLocalIdentity->name()->setString(mLocalNameBox->text());
}

void LocalIdentityWidget::retrieveNewLocalName()
{
	mLocalNameBox->setText(mLocalIdentity->name()->string());
}

void LocalIdentityWidget::selectNewPixmap()
{
	QString pixmapFileName = QFileDialog::getOpenFileName(this, "Select Picture", QString(), "PNG Image (*.png)");
	if(!pixmapFileName.isNull())
	{
		mLocalIdentity->picture()->loadPixmap(pixmapFileName);
	}
}

void LocalIdentityWidget::updatePixmap()
{
	mLocalPixmapButton->setIcon(QIcon(mLocalIdentity->picture()->pixmap()));
}
