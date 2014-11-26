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

	if(messageByteArray.size() < mMessageFlags.endpointListSize())
	{
		throw runtime_error("Failed to parse message. It has a smaller number of bytes than the EndpointListSize suggests.");
	}

	QString endpointList = QString::fromUtf8(messageByteArray.left(mMessageFlags.endpointListSize()));
	if(endpointList.contains(LIST_DELIMITER))
	{
		setTargetEndpoints(endpointList.split(LIST_DELIMITER));
	}
	else
	{
		setTargetEndpoints(QStringList(mTargetEndpoints));
	}
	messageByteArray.remove(0, mMessageFlags.endpointListSize());

	if(messageByteArray.size() <= 0)
	{
		throw runtime_error("Failed to parse message. Data section is empty.");
	}
	mMessageData = messageByteArray;
	mSender = sender;
}
ChatMessage::ChatMessage(MessageType type, QStringList targetEndpoints, QString message)
{
	mMessageData = message.toUtf8();
	setTargetEndpoints(targetEndpoints);
	mMessageFlags.setType(type);
}

ChatMessage::ChatMessage(MessageType type, QStringList targetEndpoints, QByteArray messageData)
	: mMessageData(messageData)
{
	setTargetEndpoints(targetEndpoints);
	mMessageFlags.setType(type);
}
QStringList ChatMessage::targetEndpoints() const
{
	return mTargetEndpoints;
}

void ChatMessage::setTargetEndpoints(QStringList newTargetEndpoints)
{
	mTargetEndpoints = newTargetEndpoints;
	mTargetEnpointsArray = mTargetEndpoints.join(LIST_DELIMITER).toUtf8();
	mMessageFlags.setEndpointListSize(mTargetEnpointsArray.size());
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
	messageByteArray += mTargetEnpointsArray;
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
