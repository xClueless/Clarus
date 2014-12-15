#include "LoginWidget.hpp"

#include <QMessageBox>
#include <stdexcept>
#include <iostream>

using namespace std;

LoginWidget::LoginWidget(EndpointManager* endpointManager, QWidget* parent)
	: QWidget(parent), mEndpointManager(endpointManager)
{
	mLayout = new QFormLayout(this);
	setLayout(mLayout);

	createCredentialBox();
	mLayout->addWidget(mCredentialBox);

	mLoginButton = new QPushButton("Login", this);
	connect(mLoginButton, SIGNAL(clicked()), this, SLOT(login()));
	mLayout->addWidget(mLoginButton);

	mMainWindow = new MainWindow(endpointManager);
}

LoginWidget::~LoginWidget()
{
	delete mMainWindow;
}

void LoginWidget::createCredentialBox()
{
	mCredentialBox = new QGroupBox(this);
	QHBoxLayout* credentialBoxLayout = new QHBoxLayout();

	mLocalNameLabel = new QLabel("Name:", mCredentialBox);
	credentialBoxLayout->addWidget(mLocalNameLabel);

	mLocalNameField = new QLineEdit(mCredentialBox);
	credentialBoxLayout->addWidget(mLocalNameField);
	connect(mLocalNameField, SIGNAL(returnPressed()), this, SLOT(login()));

	mCredentialBox->setLayout(credentialBoxLayout);
}

MainWindow* LoginWidget::mainWindow()
{
	return mMainWindow;
}

void LoginWidget::login()
{
	QString localName = mLocalNameField->text();
	if(localName.isEmpty())
	{
		QMessageBox::critical(this, "Login Error", "You must enter a name to login.");
	}
	else
	{
		mEndpointManager->setLocalName(localName);
		try
		{
			mEndpointManager->start();
		}
		catch(const runtime_error& re)
		{
			cerr << re.what() << endl;
			cerr << "[LoginWidget] This should probably be handled via signals and slot." << endl;
		}

		hide();
		mMainWindow->show();
	}
}
