#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QWidget>
#include <QBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include "../Network/ClientManager.hpp"
#include "LocalIdentityWidget.hpp"
#include "EndpointListWidget.hpp"

class MainWindow : public QWidget
{
private:
	Q_OBJECT
	ClientManager* mClientManager;
	QVBoxLayout* mLayout;
	LocalIdentityWidget* mLocalIdentityWidget;
	QPushButton* mConnectToEndpointButton;
	QPushButton* mBroadcastForEndpointsButton;
	EndpointListWidget* mEndpointListWidget;
public:
	explicit MainWindow(ClientManager* clientManager, QWidget *parent = 0);
	virtual ~MainWindow();

signals:

public slots:
	void connectToEndpoint();
	void endpointFailedToConnect(MessageEndpoint* endpoint, ConnectionError connectionError);
};

#endif // MAINWINDOW_HPP
