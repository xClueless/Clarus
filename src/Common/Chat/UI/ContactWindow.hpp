#ifndef CONTACTWINDOW_HPP
#define CONTACTWINDOW_HPP

#include <QWidget>
#include <QString>
#include <QLineEdit>
#include <QGridLayout>
#include <QPushButton>
#include <QSet>
#include "../Network/ClientManager.hpp"
#include "ChatWindow.hpp"
#include "EndpointButton.hpp"

class ContactWindow : public QWidget
{
private:
	QGridLayout* mLayout;
	QList<ChatWindow*> mChatWindows;
	QList<ChatGroup*> mChatGroups;
	QList<EndpointButton*> mEndpointButtons;
	ClientManager* mClientManager;

	Q_OBJECT
public:
	explicit ContactWindow(ClientManager* clientManager, QWidget *parent = 0);

signals:
	void connectToEndpoint(QString remoteHostname);
public slots:
	void endpointButtonClicked();
	void endpointIdentified(MessageEndpoint* endpoint);
	void openNewConversation(ChatMessage* message);
	void openConversation(ChatGroup* group);
};

#endif // CONTACTWINDOW_HPP
