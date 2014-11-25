#ifndef CHATGROUP_HPP
#define CHATGROUP_HPP

#include <QObject>
#include <QSet>
#include <QStringList>
#include "Network/MessageEndpoint.hpp"
#include "Network/ClientManager.hpp"

class ChatGroup : public QObject
{
private:
	Q_OBJECT
	QString mGroupName;
	ClientManager* mClientManager;
	QSet<MessageEndpoint*> mEndpoints;
public:
	explicit ChatGroup(ClientManager* clientManager, MessageEndpoint* endpoint, QObject *parent = 0);
	explicit ChatGroup(ClientManager* clientManager, QSet<MessageEndpoint*> endpoints, QObject *parent = 0);
	QSet<MessageEndpoint*> endpoints();
	void addEndpoint(MessageEndpoint* endpoint);
	void removeEndpoint(MessageEndpoint* endpoint);
	bool empty();
	QStringList endpointRemoteNames();
	QString groupName();
signals:
	void messageReady(ChatMessage* m);
	void remoteNameChanged();
	void groupNameChanged();
protected slots:
	void endpointHasNewMessage(ChatMessage* m);
	void endpointNameHasChanged();
public slots:
	void messageAll(QString messageString);
	void messageAll(ChatMessage* message);
	void setGroupName(QString name);
};

#endif // CHATGROUP_HPP
