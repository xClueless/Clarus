#include "ChatResource.hpp"

#include <QDebug>

void ChatResource::sendStateUpdate()
{
	ChatMessage cm(ChatMessage::RESOURCE_EXCHANGE);
	ResourceMessage rm(&cm, mResourceName, mResourceState);
	qDebug() << "[ChatResource:" << mResourceName << "] Sending state update";
	writeChatMessage(&cm);
}

void ChatResource::sendDataUpdate()
{
	ChatMessage cm(ChatMessage::RESOURCE_EXCHANGE);
	ResourceMessage rm(&cm, mResourceName, mResourceData);
	qDebug() << "[ChatResource: " << mResourceName << "] Sending data update";
	writeChatMessage(&cm);
}

ChatResource::ChatResource(QString resourceName, QObject* parent)
	: MessageHandler("ChatResource", parent), mResourceName(resourceName)
{
}

ChatResource::~ChatResource()
{
}

QByteArray& ChatResource::data()
{
	return mResourceData;
}
ChatResource::ResourceState ChatResource::state() const
{
	return mResourceState;
}

QString ChatResource::resourceName() const
{
	return mResourceName;
}

void ChatResource::setData(QByteArray& bytes)
{
	qDebug() << "[ChatResource:" << mResourceName << "] Data changed";
	mResourceData = bytes;
	emit dataChanged();
}
void ChatResource::setState(ChatResource::ResourceState state)
{
	qDebug() << "[ChatResource:" << mResourceName << "] State changed from" <<mResourceState << "to" << state;
	mResourceState = state;
	emit stateChanged();
}

void ChatResource::handleMessage(ChatMessage* m)
{
	ResourceMessage rm(m);
	handleResourceMessage(&rm);
}
