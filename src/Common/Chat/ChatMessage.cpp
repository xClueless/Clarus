#include "ChatMessage.hpp"
#include <stdexcept>
#include <iostream>
#include <QDataStream>
#include <QJsonArray>

using namespace std;

ChatMessage::ChatMessage(QJsonDocument json, QString sender) : mSender(sender)
{
	QJsonObject rootObject = json.object();
	mSectionMap = rootObject.toVariantMap();

	if(!rootObject.contains("API-Level"))
	{
		throw runtime_error("[ChatMessage] Cannot parse. Remote did not provide an API level.");
	}
	if(!rootObject.contains("MessageType"))
	{
		throw runtime_error("[ChatMessage] Cannot parse. Remote did not provide a message type.");
	}

	if(mSectionMap["API-Level"].toInt() > LOCAL_API_LEVEL)
	{
		throw runtime_error("[ChatMessage] Cannot parse. Remote has a higher API level.");
	}

	mMessageType = (MessageType) rootObject["MessageType"].toInt();
}
ChatMessage::ChatMessage(MessageType type) : mMessageType(type)
{
	addSection("API-Level", QVariant(LOCAL_API_LEVEL));
	addSection("Message-Type", QVariant(mMessageType));

}
void ChatMessage::addSection(QString sectionName, const QVariant data)
{
	mSectionMap[sectionName] = data;
}

QVariantMap& ChatMessage::sections()
{
	return mSectionMap;
}

QVariant& ChatMessage::section(QString sectionName)
{
	return mSectionMap[sectionName];
}
const QJsonDocument ChatMessage::jsonDocument()
{
	QJsonDocument json = QJsonDocument(QJsonObject::fromVariantMap(mSectionMap));
	return json;
}
QString ChatMessage::sender()
{
	return mSender;
}
void ChatMessage::setSender(QString sender)
{
	mSender = sender;
}
ChatMessage::MessageType ChatMessage::type()
{
	return mMessageType;
}

void ChatMessage::setType(ChatMessage::MessageType type)
{
	mMessageType = type;
}
