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
	static const char RAW_CHAR = 'R';
	static const char INTERNAL_CHAR = 'I';
	static const char PRIVATE_CHAR = 'P';

	MessageType mType = RAW;
	quint16 mEndpointListSize = 0;

	char typeAsChar() const;
	MessageType typeFromChar(char typeCharacter) const;
public:
	static const size_t FLAG_SECTION_BYTES = sizeof(char) + sizeof(mEndpointListSize);
	MessageFlags();
	MessageFlags(MessageType type, quint16 endpointListSize);
	MessageFlags(QByteArray flagBytes);

	MessageType type() const;
	void setType(MessageType type);

	quint16 endpointListSize() const;
	void setEndpointListSize(quint16 listSize);

	QString flagString() const;
	QByteArray flagBytes() const;
};

#endif // MESSAGEFLAGS_HPP
