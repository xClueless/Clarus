#ifndef LOGINWIDGET_HPP
#define LOGINWIDGET_HPP

#include <QWidget>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QGroupBox>
#include <QBoxLayout>
#include <QFormLayout>
#include "../Network/EndpointManager.hpp"
#include "MainWindow.hpp"

class LoginWidget : public QWidget
{
private:
	Q_OBJECT
	EndpointManager* mEndpointManager;
	QFormLayout* mLayout;
	QLabel* mLocalNameLabel;
	QLineEdit* mLocalNameField;
	QGroupBox* mCredentialBox;
	QPushButton* mLoginButton;
	MainWindow* mMainWindow;
	LocalIdentity* mLocalIdentity;

public:
	explicit LoginWidget(LocalIdentity* identity, EndpointManager* endpointManager, QWidget *parent = 0);
	virtual ~LoginWidget();
	void createCredentialBox();
	MainWindow* mainWindow();
signals:

public slots:
	void login();
};

#endif // LOGINWIDGET_HPP
