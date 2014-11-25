#ifndef ENDPOINTLISTWIDGET_HPP
#define ENDPOINTLISTWIDGET_HPP

#include <QWidget>
#include <QString>
#include <QLineEdit>
#include <QGridLayout>
#include <QPushButton>
#include <QSet>
#include "../Network/ClientManager.hpp"
#include "ChatWindow.hpp"
#include "EndpointButton.hpp"

class EndpointListWidget : public QWidget
{
private:
	QGridLayout* mLayout;
	QList<ChatWindow*> mChatWindows;
	QList<ChatGroup*> mChatGroups;
	QList<EndpointButton*> mEndpointButtons;
	ClientManager* mClientManager;

	Q_OBJECT
public:
	explicit EndpointListWidget(ClientManager* clientManager, QWidget *parent = 0);
	virtual ~EndpointListWidget();

signals:
	void connectToEndpoint(QString remoteHostname);
public slots:
	void endpointButtonClicked();
	void endpointIdentified(MessageEndpoint* endpoint);
	void openNewConversation(ChatMessage* message);
	void openConversation(ChatGroup* group);
};

#endif // ENDPOINTLISTWIDGET_HPP
