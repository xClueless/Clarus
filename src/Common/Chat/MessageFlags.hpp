#ifndef MESSAGEFLAGS_HPP
#define MESSAGEFLAGS_HPP

#include <QString>
#include <QByteArray>

enum MessageType
{
	RAW,
	INTERNAL,
	PRIVATE
};

class MessageFlags
{
private:
	QChar RAW_CHAR = 'R';
	QChar INTERNAL_CHAR = 'I';
	QChar PRIVATE_CHAR = 'P';

	MessageType mType = RAW;

	QChar typeAsChar() const;
	MessageType typeFromChar(QChar typeCharacter) const;
public:
	static const size_t FLAG_SECTION_BYTES = 2; //Depending on QChar being two bytes long is probably bad.
	MessageFlags();
	MessageFlags(MessageType type);
	MessageFlags(QByteArray flagBytes);

	MessageType type() const;
	void setType(MessageType type);

	QString flagString() const;
	QByteArray flagBytes() const;
};

#endif // MESSAGEFLAGS_HPP
