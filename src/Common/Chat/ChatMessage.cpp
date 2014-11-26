#include "ChatMessage.hpp"
#include <stdexcept>
#include <iostream>
#include <QDataStream>

using namespace std;

ChatMessage::ChatMessage(QByteArray messageByteArray, QString sender)
{
	if(messageByteArray.size() < MINIMUM_MESSAGE_BYTES)
	{
		throw runtime_error("Failed to parse message. It does not have the minimum number of bytes required.");
	}

	mMessageFlags = MessageFlags(messageByteArray.left(MessageFlags::FLAG_SECTION_BYTES));
	messageByteArray.remove(0, MessageFlags::FLAG_SECTION_BYTES);

	if(messageByteArray.size() <= 0)
	{
		throw runtime_error("Failed to parse message. Data section is empty.");
	}
	mMessageData = messageByteArray;
	mSender = sender;
}
ChatMessage::ChatMessage(MessageType type, QString message) : mMessageFlags(type)
{
	mMessageData = message.toUtf8();
}

ChatMessage::ChatMessage(MessageType type, QByteArray messageData)
	: mMessageData(messageData), mMessageFlags(type)
{
}
QString ChatMessage::messageAsUTF8String() const
{
	if(mMessageFlags.type() == RAW)
	{
		return "RAW_MESSAGE";
	}
	return QString::fromUtf8(mMessageData);
}

QByteArray ChatMessage::messageBytes()
{
	QByteArray messageByteArray;
	messageByteArray += mMessageFlags.flagBytes();
	messageByteArray += mMessageData;
	return messageByteArray;
}

QString ChatMessage::sender()
{
	return mSender;
}

void ChatMessage::setSender(QString sender)
{
	mSender = sender;
}

MessageFlags ChatMessage::flags()
{
	return mMessageFlags;
}
