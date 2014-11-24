#ifndef MESSAGEFLAGS_HPP
#define MESSAGEFLAGS_HPP

#include <QString>

enum MessageType
{
	INTERNAL = 0,
	PRIVATE = 1
};

class MessageFlags
{
private:
	QChar INTERNAL_CHAR = 'I';
	QChar PRIVATE_CHAR = 'P';

	MessageType mType;
public:
	MessageFlags(MessageType type=INTERNAL);
	MessageFlags(QString flagString);
	MessageType type();
	void setType(MessageType type);
	QString flagString();
};

#endif // MESSAGEFLAGS_HPP
