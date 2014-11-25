#ifndef CHATMESSAGE_HPP
#define CHATMESSAGE_HPP

#include <QString>
#include <QStringList>
#include <QChar>
#include "MessageFlags.hpp"

class ChatMessage
{
private:
	const char DELIMITER = ':';
	const char LIST_DELIMITER = ',';
	const int MINIMUM_PARTS = 4;

	QStringList mTargetEndpoints;
	QString mMessage;
	QString mSender = "SENDER_DEFAULT";
	MessageFlags mMessageFlags;
public:
	ChatMessage(QString messageString, QString sender);
	ChatMessage(MessageFlags flags, QStringList targetEndpoints, QString message);
	QStringList targetEndpoints() const;
	QString message() const;
	QString messageString();
	QString sender();
	void setSender(QString sender);
	MessageFlags flags();
};

#endif // CHATMESSAGE_HPP
