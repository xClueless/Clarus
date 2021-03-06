#include "ChatMessage.hpp"
#include <stdexcept>
#include <iostream>
#include <QDataStream>

using namespace std;

ChatMessage::ChatMessage(QByteArray messageByteArray, QString sender)
{
	if(messageByteArray.size() < MINIMUM_MESSAGE_BYTES)
	{
		QString errorString = "[ChatMessage] Failed to parse message. It only has " + QString::number(mMessageData.size()) + " bytes however " + QString::number(MINIMUM_MESSAGE_BYTES) + " bytes are required.";
		throw runtime_error(errorString.toStdString());
	}

	mMessageFlags = MessageFlags(messageByteArray.left(MessageFlags::FLAG_SECTION_BYTES));
	messageByteArray.remove(0, MessageFlags::FLAG_SECTION_BYTES);

	if(messageByteArray.size() <= 0)
	{
		throw runtime_error("[ChatMessage] Failed to parse message. Data section is empty.");
	}
	mMessageData = messageByteArray;
	mSender = sender;
}
ChatMessage::ChatMessage(MessageFlags flags, QString message) : mMessageFlags(flags)
{
	mMessageData = message.toUtf8();
}

ChatMessage::ChatMessage(MessageFlags flags, QByteArray messageData)
	: mMessageData(messageData), mMessageFlags(flags)
{
}
QString ChatMessage::messageDataAsUTF8String() const
{
	if(mMessageFlags.format() == MessageFormat::RAW)
	{
		return "RAW_MESSAGE";
	}
	return QString::fromUtf8(mMessageData);
}
QByteArray ChatMessage::messageData() const
{
	return mMessageData;
}

QByteArray ChatMessage::rawMessageBytes()
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
