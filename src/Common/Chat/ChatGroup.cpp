#include "ChatGroup.hpp"

#include <iostream>

using namespace std;

ChatGroup::ChatGroup(EndpointManager* endpointManager, MessageEndpoint* endpoint, QObject* parent)
	: QObject(parent), mEndpointManager(endpointManager), mGroupPixmap(DEFAULT_GROUP_ICON_PATH)
{
	addEndpoint(endpoint);
}

ChatGroup::ChatGroup(EndpointManager* endpointManager, QSet<MessageEndpoint*> endpoints, QObject *parent)
	: QObject(parent), mEndpointManager(endpointManager), mGroupPixmap(DEFAULT_GROUP_ICON_PATH)
{
	for(MessageEndpoint* endpoint : endpoints)
	{
		addEndpoint(endpoint);
		emit endpointAdded(endpoint);
	}
}

ChatGroup::~ChatGroup()
{
}

QList<MessageEndpoint*> ChatGroup::endpoints()
{
	return mEndpoints;
}

void ChatGroup::addEndpoint(MessageEndpoint* endpoint)
{
	mEndpoints << endpoint;
	connect(endpoint, SIGNAL(messageReady(ChatMessage*)), this, SLOT(endpointHasNewMessage(ChatMessage*)));

	connect(endpoint->identity()->name(), SIGNAL(dataChanged()), this, SLOT(endpointNameHasChanged()));
	connect(endpoint->identity()->picture(), SIGNAL(dataChanged()), this, SLOT(endpointPictureHasChanged()));
}

void ChatGroup::removeEndpoint(MessageEndpoint* endpoint)
{
	mEndpoints.removeOne(endpoint);
	emit endpointRemoved(endpoint);
}

bool ChatGroup::empty()
{
	return mEndpoints.empty();
}

QStringList ChatGroup::endpointRemoteNames()
{
	QStringList endpointNames;

	for(MessageEndpoint* endpoint : mEndpoints)
	{
		endpointNames << endpoint->identity()->name()->string();
	}
	return endpointNames;
}

QString ChatGroup::groupName()
{
	if(mGroupName.isEmpty())
	{
		return endpointRemoteNames().join(',');
	}
	else
	{
		return mGroupName;
	}
}

const QPixmap& ChatGroup::groupPixmap()
{
	if(mEndpoints.size() == 1)
	{
		return mEndpoints.front()->identity()->picture()->pixmap();
	}
	return mGroupPixmap;
}

void ChatGroup::endpointHasNewMessage(ChatMessage* m)
{
	emit messageReady(m);
}

void ChatGroup::endpointNameHasChanged()
{
	if(groupName().isEmpty())
	{
		emit groupNameChanged();
	}
}

void ChatGroup::endpointPictureHasChanged()
{
	if(mEndpoints.size() == 1)
	{
		setGroupPixmap(mEndpoints.front()->identity()->picture()->pixmap());
	}
}

void ChatGroup::messageAll(QString messageString)
{
	if(!empty())
	{
		ChatMessage message(ChatMessage::PRIVATE);
		message.addSection("Message", messageString);

		for(MessageEndpoint* endpoint : mEndpoints)
		{
			endpoint->writeChatMessage(&message);
		}
	}
}

void ChatGroup::messageAll(ChatMessage* message)
{
	if(!empty())
	{
		for(MessageEndpoint* endpoint : mEndpoints)
		{
			endpoint->writeChatMessage(message);
		}
	}
}

void ChatGroup::setGroupName(QString name)
{
	mGroupName = name;
}

void ChatGroup::setGroupPixmap(QPixmap pixmap)
{
	mGroupPixmap = pixmap;
	emit groupPixmapChanged();
}

