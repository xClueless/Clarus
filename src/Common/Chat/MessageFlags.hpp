#ifndef MESSAGEFLAGS_HPP
#define MESSAGEFLAGS_HPP

#include <QString>
#include <QByteArray>

enum class MessageType
{
	IDENTIFICATION,
	PIXMAP_EXCHANGE,
	PRIVATE,
	PROTOCOL_ERROR
};
enum class MessageFormat
{
	RAW,
	UTF8
};

class MessageFlags
{
private:
	QChar RAW_CHAR = 'R';
	QChar UTF8_CHAR = '8';

	QChar INDENTIFICATION_CHAR = 'I';
	QChar ENDPOINT_PIXMAP_EXCHANGE_CHAR = 'X';
	QChar PRIVATE_CHAR = 'P';
	QChar PROTOCOL_ERROR_CHAR = 'E';

	MessageType mType = MessageType::PRIVATE;
	MessageFormat mFormat = MessageFormat::UTF8;

	QChar typeAsChar() const;
	MessageType typeFromChar(QChar typeCharacter) const;

	QChar formatAsChar() const;
	MessageFormat formatFromChar(QChar formatCharacter) const;
public:
	//QChar(2) * 2 = 4 Depending on QChar being two bytes long is probably bad.
	static const size_t FLAG_SECTION_BYTES = 4;
	MessageFlags();
	MessageFlags(MessageType type, MessageFormat format);
	MessageFlags(QByteArray flagBytes);

	MessageType type() const;
	void setType(MessageType type);

	MessageFormat format() const;
	void setFormat(MessageFormat format);

	QString flagString() const;
	QByteArray flagBytes() const;
};

#endif // MESSAGEFLAGS_HPP
