#ifndef CHATRESOURCE_HPP
#define CHATRESOURCE_HPP

#include "../ChatMessage.hpp"
#include "../Network/MessageHandler.hpp"
#include "ResourceMessage.hpp"

#include <QObject>
#include <QPixmap>

class ResourceMessage;

class ChatResource : public MessageHandler
{
public:
	enum ResourceState
	{
		NOT_AVAILABLE,
		STALE,
		READY_TO_RECIEVE_DATA,
		UPDATE_AVAILABLE,
		FRESH
	};
private:
	Q_OBJECT
	const QString RESOURCE_STALE_STRING = "RESOURCE_STALE";
	const QString RESOURCE_REQUEST_STRING = "RESOURCE_REQUEST";
protected:
	QString mResourceName;
	QByteArray mResourceData;
	ResourceState mResourceState;

	void sendStateUpdate();
	void sendDataUpdate();

public:
	explicit ChatResource(QString resourceName, QObject *parent = 0);
	~ChatResource();
	QByteArray& data();
	ResourceState state() const;
	QString resourceName() const;
protected:
	void setData(QByteArray& bytes);
	void setState(ResourceState state);
signals:
	void stateChanged();
	void dataChanged();
public slots:
	void handleMessage(ChatMessage* m);
	virtual void handleResourceMessage(ResourceMessage* rm)=0;
};

#endif // CHATPIXMAP_HPP
