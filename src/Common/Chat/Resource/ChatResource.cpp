#include "ChatResource.hpp"

void ChatResource::sendStateUpdate()
{
	ChatMessage cm(ChatMessage::RESOURCE_EXCHANGE);
	ResourceMessage rm(&cm, mResourceName, mResourceState);
	writeChatMessage(&cm);
}

void ChatResource::sendDataUpdate()
{
	ChatMessage cm(ChatMessage::RESOURCE_EXCHANGE);
	ResourceMessage rm(&cm, mResourceName, mResourceData);
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

QString ChatResource::utf8Data()
{
	return QString::fromUtf8(mResourceData);
}

ChatResource::ResourceState ChatResource::state()
{
	return mResourceState;
}

void ChatResource::setData(QByteArray& bytes)
{
	mResourceData = bytes;
	emit resourceChanged();
}
void ChatResource::setDataUtf8(QString& utf8)
{
	QByteArray data = utf8.toUtf8();
	setData(data);
}

void ChatResource::setState(ChatResource::ResourceState state)
{
	mResourceState = state;
	emit stateChanged();
}

void ChatResource::handleMessage(ChatMessage* m)
{
	ResourceMessage rm(m);
	handleResourceMessage(&rm);
}
