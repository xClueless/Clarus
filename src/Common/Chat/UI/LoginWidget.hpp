#ifndef LOGINWIDGET_HPP
#define LOGINWIDGET_HPP

#include <QWidget>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QGroupBox>
#include <QBoxLayout>
#include <QFormLayout>
#include "../Network/ClientManager.hpp"
#include "MainWindow.hpp"

class LoginWidget : public QWidget
{
private:
	Q_OBJECT
	ClientManager* mClientManager;
	QFormLayout* mLayout;
	QLabel* mLocalNameLabel;
	QLineEdit* mLocalNameField;
	QGroupBox* mCredentialBox;
	QPushButton* mLoginButton;
	MainWindow* mMainWindow;

public:
	explicit LoginWidget(ClientManager* clientManager, QWidget *parent = 0);
	virtual ~LoginWidget();
	void createCredentialBox();
	MainWindow* mainWindow();
signals:

public slots:
	void login();
};

#endif // LOGINWIDGET_HPP
