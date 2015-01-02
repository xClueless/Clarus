#include "MainWindow.hpp"

#include <QInputDialog>
#include <QMessageBox>

MainWindow::MainWindow(LocalIdentity* identity, EndpointManager* endpointManager, QWidget* parent)
	: QWidget(parent), mEndpointManager(endpointManager), mLocalIdentity(identity)
{
	connect(mEndpointManager, SIGNAL(connectionFailed(MessageEndpoint*,ConnectionError)), this, SLOT(endpointFailedToConnect(MessageEndpoint*, ConnectionError)));

	mLayout = new QVBoxLayout(this);
	setLayout(mLayout);

	mLocalIdentityWidget = new LocalIdentityWidget(mLocalIdentity, this);
	mLayout->addWidget(mLocalIdentityWidget);

	mConnectToEndpointButton = new QPushButton("+", this);
	connect(mConnectToEndpointButton, SIGNAL(clicked()), this, SLOT(connectToEndpoint()));
	mLayout->addWidget(mConnectToEndpointButton);

	mBroadcastForEndpointsButton = new QPushButton("Find Local Clients", this);
	connect(mBroadcastForEndpointsButton, SIGNAL(clicked()), mEndpointManager, SLOT(sendBroadcast()));
	mLayout->addWidget(mBroadcastForEndpointsButton);

	mEndpointListWidget = new EndpointListWidget(mEndpointManager, this);
	mLayout->addWidget(mEndpointListWidget);
}

MainWindow::~MainWindow()
{
}

void MainWindow::connectToEndpoint()
{
	QString endpointRemoteName = QInputDialog::getText(this, "Connect to Endpoint", "Endpoint IP/Hostname");
	mEndpointManager->connectToEndpoint(endpointRemoteName);
}

void MainWindow::endpointFailedToConnect(MessageEndpoint* endpoint, ConnectionError connectionError)
{
	QMessageBox::critical(this, "Failed to connect", "Failed to connect to endpoint: " + connectionError.errorString());
}
