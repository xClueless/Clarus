#include "MainWindow.hpp"

#include <QInputDialog>
#include <QMessageBox>

MainWindow::MainWindow(ClientManager* clientManager, QWidget* parent)
	: QWidget(parent), mClientManager(clientManager)
{
	connect(mClientManager, SIGNAL(failedToConnectToEndpoint(MessageEndpoint*, ConnectionError)), this, SLOT(endpointFailedToConnect(MessageEndpoint*, ConnectionError)));

	mLayout = new QVBoxLayout(this);
	setLayout(mLayout);

	mLocalIdentityWidget = new LocalIdentityWidget(mClientManager, this);
	mLayout->addWidget(mLocalIdentityWidget);

	mConnectToEndpointButton = new QPushButton("+", this);
	connect(mConnectToEndpointButton, SIGNAL(clicked()), this, SLOT(connectToEndpoint()));
	mLayout->addWidget(mConnectToEndpointButton);

	mEndpointListWidget = new EndpointListWidget(mClientManager, this);
	mLayout->addWidget(mEndpointListWidget);
}

MainWindow::~MainWindow()
{
}

void MainWindow::connectToEndpoint()
{
	QString endpointRemoteName = QInputDialog::getText(this, "Connect to Endpoint", "Endpoint IP/Hostname");
	mClientManager->connectToServer(endpointRemoteName);
}

void MainWindow::endpointFailedToConnect(MessageEndpoint* endpoint, ConnectionError connectionError)
{
	QMessageBox::critical(this, "Failed to connect", "Failed to connect to endpoint: " + connectionError.errorString());
}
