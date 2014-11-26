#ifndef CHATMESSAGE_HPP
#define CHATMESSAGE_HPP

#include <QString>
#include <QStringList>
#include <QChar>
#include <QByteArray>
#include "MessageFlags.hpp"

class ChatMessage
{
private:
	const char LIST_DELIMITER = ',';
	const int MINIMUM_MESSAGE_BYTES = MessageFlags::FLAG_SECTION_BYTES;

	QStringList mTargetEndpoints;
	QByteArray mTargetEnpointsArray;

	QByteArray mMessageData;
	QString mSender = "SENDER_DEFAULT";
	MessageFlags mMessageFlags;

	void processRawData(QByteArray rawData);
	void processStringData(QByteArray messageByteArray);
public:
	ChatMessage(QByteArray messageByteArray, QString sender);
	ChatMessage(MessageType type, QStringList targetEndpoints, QString message);
	ChatMessage(MessageType type, QStringList targetEndpoints, QByteArray messageData);

	QStringList targetEndpoints() const;
	void setTargetEndpoints(QStringList newTargetEndpoints);

	QString sender();
	void setSender(QString sender);

	QString messageAsUTF8String() const;
	QByteArray messageBytes();
	MessageFlags flags();
};

#endif // CHATMESSAGE_HPP
