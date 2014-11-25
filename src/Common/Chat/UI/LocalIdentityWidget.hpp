#ifndef LOCALIDENTITYWIDGET_HPP
#define LOCALIDENTITYWIDGET_HPP

#include <QWidget>
#include <QBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include "../Network/ClientManager.hpp"

class LocalIdentityWidget : public QWidget
{
private:
	Q_OBJECT
	ClientManager* mClientManager;
	QHBoxLayout* mLayout;
	QLabel* mLocalNameLabel;
	QLineEdit* mLocalNameBox;
public:
	explicit LocalIdentityWidget(ClientManager* clientManager, QWidget *parent = 0);

signals:

public slots:
	void sendNewLocalName();
	void retrieveNewLocalName();
};

#endif // LOCALIDENTITYWIDGET_HPP
