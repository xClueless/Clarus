#include "ChatMessage.hpp"
#include <stdexcept>
#include <iostream>

using namespace std;

ChatMessage::ChatMessage(QString rawMessage, QString sender)
{
//	cout << "[ChatMessage] Parsing raw message: " << rawMessage.toStdString() << endl;
	int messageParts = rawMessage.count(DELIMITER) + 2; //Have to account for the start and end parts.
	if(messageParts < MINIMUM_PARTS)
	{
		QString errorString("Failed to parse chat message. Message does not contain enough parts. Has " + QString::number(messageParts) + " should have " + QString::number(MINIMUM_PARTS));
		errorString += " Raw message: " + rawMessage;
		throw runtime_error(errorString.toStdString());
	}

	QString flagSection = rawMessage.section(DELIMITER, 0, 0);
	mMessageFlags = MessageFlags(flagSection);

	QString targetEndpointString = rawMessage.section(DELIMITER, 1, 1);

	if(targetEndpointString.contains(LIST_DELIMITER))
	{
		mTargetEndpoints = targetEndpointString.split(LIST_DELIMITER);
	}
	else
	{
		mTargetEndpoints << targetEndpointString;
	}
	mMessage = rawMessage.section(DELIMITER, 2);
	mSender = sender;
}
ChatMessage::ChatMessage(MessageFlags flags, QStringList targetEndpoints, QString message) : mMessageFlags(flags), mTargetEndpoints(targetEndpoints), mMessage(message)
{
}

QStringList ChatMessage::targetEndpoints() const
{
	return mTargetEndpoints;
}
QString ChatMessage::message() const
{
	return mMessage;
}

QString ChatMessage::messageString()
{
	return mMessageFlags.flagString() + DELIMITER + mTargetEndpoints.join(',') + DELIMITER + mMessage;
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
