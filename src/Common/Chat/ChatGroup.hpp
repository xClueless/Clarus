#ifndef CHATGROUP_HPP
#define CHATGROUP_HPP

#include <QObject>
#include <QList>
#include <QStringList>
#include <QPixmap>
#include "Network/MessageEndpoint.hpp"
#include "Network/EndpointManager.hpp"

class ChatGroup : public QObject
{
private:
	Q_OBJECT
	const QString DEFAULT_GROUP_ICON_PATH = ":/images/default-contact-icon.png";

	QString mGroupName;
	QPixmap mGroupPixmap;
	EndpointManager* mEndpointManager;
	QList<MessageEndpoint*> mEndpoints;
public:
	explicit ChatGroup(EndpointManager* endpointManager, MessageEndpoint* endpoint, QObject *parent = 0);
	explicit ChatGroup(EndpointManager* endpointManager, QSet<MessageEndpoint*> endpoints, QObject *parent = 0);
	virtual ~ChatGroup();
	QList<MessageEndpoint*> endpoints();
	void addEndpoint(MessageEndpoint* endpoint);
	void removeEndpoint(MessageEndpoint* endpoint);
	bool empty();

	QStringList endpointRemoteNames();
	QString groupName();
	const QPixmap& groupPixmap();
signals:
	void messageReady(ChatMessage* m);
	void groupNameChanged();
	void groupPixmapChanged();
	void endpointRemoved(MessageEndpoint* endpoint);
	void endpointAdded(MessageEndpoint* endpoint);
protected slots:
	void endpointHasNewMessage(ChatMessage* m);
	void endpointNameHasChanged();
	void endpointPictureHasChanged();

public slots:
	void messageAll(QString messageString);
	void messageAll(ChatMessage* message);
	void setGroupName(QString name);
	void setGroupPixmap(QPixmap pixmap);
};

#endif // CHATGROUP_HPP
