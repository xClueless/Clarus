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

	QByteArray mMessageData;
	QString mSender = "SENDER_DEFAULT";
	MessageFlags mMessageFlags;

	void processRawData(QByteArray rawData);
	void processStringData(QByteArray messageByteArray);
public:
	ChatMessage(QByteArray messageByteArray, QString sender);
	ChatMessage(MessageFlags flags, QString message);
	ChatMessage(MessageFlags flags, QByteArray messageData);

	QString sender();
	void setSender(QString sender);

	QString messageAsUTF8String() const;
	QByteArray messageBytes();
	MessageFlags flags();
};

#endif // CHATMESSAGE_HPP
