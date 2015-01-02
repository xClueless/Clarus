#include "ResourceMessage.hpp"
#include "ChatResource.hpp"
#include <QVariant>

ResourceMessage::ResourceMessage(ChatMessage* message, QString resourceName, int state)
	: mMessage(message)
{
	mMessage->setType(ChatMessage::MessageType::RESOURCE_EXCHANGE);
	QVariantMap resourceNode;
	resourceNode["ResourceName"] = resourceName;
	resourceNode["ResourceOperation"] = SENT_UPDATED_DATA;
	resourceNode["ResourceState"] = QVariant(state);
	mMessage->addSection("Resource", resourceNode);
}

ResourceMessage::ResourceMessage(ChatMessage* message, QString resourceName, const QByteArray& data)
	: mMessage(message)
{
	mMessage->setType(ChatMessage::MessageType::RESOURCE_EXCHANGE);
	QVariantMap resourceMap;
	resourceMap["ResourceName"] = resourceName;
	resourceMap["ResourceOperation"] = SENT_UPDATED_DATA;
	resourceMap["ResourceData"] = data;
	mMessage->addSection("Resource", resourceMap);
}

ResourceMessage::ResourceMessage(ChatMessage* message)
	: mMessage(message)
{
}

QString ResourceMessage::resourceName() const
{
	QVariantMap resourceNode = mMessage->section("Resource");
	return resourceNode["ResourceName"].toString();
}

int ResourceMessage::resourceState() const
{
	QVariantMap resourceNode = mMessage->section("Resource");
	return resourceNode["ResourceState"].toString();
}

ResourceMessage::ResourceOperation ResourceMessage::resourceOperation() const
{
	QVariantMap resourceNode = mMessage->section("Resource");
	return resourceNode["ResourceOperation"].toInt();
}

QByteArray ResourceMessage::resourceData()
{
	QVariantMap& resourceNode = mMessage->section("Resource");
	return resourceNode["ResourceData"].toByteArray();
}

ResourceMessage::~ResourceMessage()
{
}

