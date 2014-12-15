#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QWidget>
#include <QBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include "../Network/EndpointManager.hpp"
#include "LocalIdentityWidget.hpp"
#include "EndpointListWidget.hpp"

class MainWindow : public QWidget
{
private:
	Q_OBJECT
	EndpointManager* mEndpointManager;
	QVBoxLayout* mLayout;
	LocalIdentityWidget* mLocalIdentityWidget;
	QPushButton* mConnectToEndpointButton;
	QPushButton* mBroadcastForEndpointsButton;
	EndpointListWidget* mEndpointListWidget;
public:
	explicit MainWindow(EndpointManager* endpointManager, QWidget *parent = 0);
	virtual ~MainWindow();

signals:

public slots:
	void connectToEndpoint();
	void endpointFailedToConnect(MessageEndpoint* endpoint, ConnectionError connectionError);
};

#endif // MAINWINDOW_HPP
