//#include "ChatMessage.hpp"
//#include <stdexcept>

//using namespace std;

//ChatMessage::ChatMessage(QString rawMessage, QString sender)
//{
//	if(rawMessage.count(DELIMITER) != MINIMUM_PARTS)
//	{
//		throw runtime_error("Failed to parse chat message. Message does not contain enough parts.");
//	}

//	QString flagSection = rawMessage.section(QChar(DELIMITER), 0, 1);
//	mMessageFlags = MessageFlags(flagSection);

//	QString messageSection = rawMessage.section(DELIMITER, 1, 2);
//	QString targetClientString = messageSection.section(DELIMITER, 2, 3);

//	if(targetClientString.contains(LIST_DELIMITER))
//	{
//		mTargetClients = targetClientString.split(LIST_DELIMITER);
//	}
//	else
//	{
//		mTargetClients << targetClientString;
//	}
//	mMessage = rawMessage.section(DELIMITER, 3, 4);
//	mSender = sender;
//}
//ChatMessage::ChatMessage(MessageFlags flags, QStringList targetClients, QString message) : mMessageFlags(flags), mTargetClients(targetClients), mMessage(message)
//{
//}

//QStringList ChatMessage::targetClients() const
//{
//	return mTargetClients;
//}
//QString ChatMessage::message() const
//{
//	return mMessage;
//}

//QString ChatMessage::messageString()
//{
//	return mTargetClients.join(',') + '#' + mMessage;
//}

//QString ChatMessage::sender()
//{
//	return mSender;
//}

//MessageFlags ChatMessage::flags()
//{
//	return mMessageFlags;
//}
