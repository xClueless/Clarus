#ifndef ENDPOINTLISTWIDGET_HPP
#define ENDPOINTLISTWIDGET_HPP

#include <QWidget>
#include <QString>
#include <QLineEdit>
#include <QGridLayout>
#include <QPushButton>
#include <QMap>
#include "../Network/EndpointManager.hpp"
#include "ChatWindow.hpp"
#include "EndpointButton.hpp"

class EndpointListWidget : public QWidget
{
private:
	QGridLayout* mLayout;
	QList<ChatGroup*> mChatGroups;
	QMap<ChatGroup*, ChatWindow*> mChatWindows;
	QList<EndpointButton*> mEndpointButtons;
	EndpointManager* mEndpointManager;

	Q_OBJECT
public:
	explicit EndpointListWidget(EndpointManager* endpointManager, QWidget *parent = 0);
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
