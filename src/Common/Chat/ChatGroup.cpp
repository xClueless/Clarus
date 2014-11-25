#include "ChatGroup.hpp"

#include <iostream>

using namespace std;

ChatGroup::ChatGroup(ClientManager* clientManager, MessageEndpoint* endpoint, QObject* parent)
	: QObject(parent), mClientManager(clientManager)
{
	addEndpoint(endpoint);
}

ChatGroup::ChatGroup(ClientManager* clientManager, QSet<MessageEndpoint*> endpoints, QObject *parent) :
	QObject(parent), mClientManager(clientManager)
{
	for(MessageEndpoint* endpoint : endpoints)
	{
		addEndpoint(endpoint);
	}
}

QSet<MessageEndpoint*> ChatGroup::endpoints()
{
	return mEndpoints;
}

void ChatGroup::addEndpoint(MessageEndpoint* endpoint)
{
	mEndpoints << endpoint;
	connect(endpoint, SIGNAL(messageReady(ChatMessage*)), this, SLOT(endpointHasNewMessage(ChatMessage*)));
	connect(endpoint, SIGNAL(remoteNameChanged()), this, SLOT(endpointNameHasChanged()));
}

void ChatGroup::removeEndpoint(MessageEndpoint* endpoint)
{
	mEndpoints.remove(endpoint);
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
		endpointNames << endpoint->remoteName();
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

void ChatGroup::endpointHasNewMessage(ChatMessage* m)
{
	emit messageReady(m);
}

void ChatGroup::endpointNameHasChanged()
{
	emit remoteNameChanged();

	if(groupName().isEmpty())
	{
		emit groupNameChanged();
	}
}

void ChatGroup::messageAll(QString messageString)
{
	if(!empty())
	{
		ChatMessage message(MessageFlags(PRIVATE), endpointRemoteNames(), messageString);
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

