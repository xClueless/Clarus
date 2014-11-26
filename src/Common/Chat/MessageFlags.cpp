#include "MessageFlags.hpp"

#include <stdexcept>
#include <iostream>
#include <QDataStream>

using namespace std;

MessageFlags::MessageFlags()
{
}
MessageFlags::MessageFlags(const MessageType type) : mType(type)
{
}

MessageFlags::MessageFlags(QByteArray flagBytes)
{
	if(flagBytes.size() != FLAG_SECTION_BYTES)
	{
		throw runtime_error("Failed to parse flags. Flag byte array does not contain the correct number of bytes.");
	}
	QDataStream flagByteStream(flagBytes);

	QChar typeChar;
	flagByteStream >> typeChar;
	mType = typeFromChar(typeChar);
}

MessageType MessageFlags::type() const
{
	return mType;
}

void MessageFlags::setType(MessageType type)
{
	mType = type;
}

QChar MessageFlags::typeAsChar() const
{
	switch(mType)
	{
		case INTERNAL: return INTERNAL_CHAR;
		case PRIVATE: return PRIVATE_CHAR;
		case RAW: return RAW_CHAR;
		default: throw runtime_error("Unknown type flag: " + mType);
	}
}

MessageType MessageFlags::typeFromChar(QChar typeCharacter) const
{
	if(typeCharacter == INTERNAL_CHAR) return INTERNAL;
	if(typeCharacter == PRIVATE_CHAR) return PRIVATE;
	if(typeCharacter == RAW_CHAR) return RAW;
	throw runtime_error("Unknown type flag: " + typeCharacter.toLatin1());
}

QString MessageFlags::flagString() const
{
	QString flagString;
	flagString += "Type: ";
	flagString += typeAsChar();
	return flagString;
}

QByteArray MessageFlags::flagBytes() const
{
	QByteArray flagBytes;
	QDataStream flagByteStream(&flagBytes, QIODevice::WriteOnly);
	flagByteStream << typeAsChar();
	return flagBytes;
}
