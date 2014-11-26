#include "MessageFlags.hpp"

#include <stdexcept>
#include <iostream>
#include <QDataStream>

using namespace std;

MessageFlags::MessageFlags()
{
}
MessageFlags::MessageFlags(MessageType type, MessageFormat format) : mType(type)
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

	QChar formatChar;
	flagByteStream >> formatChar;
	mFormat = formatFromChar(formatChar);
}

MessageType MessageFlags::type() const
{
	return mType;
}

void MessageFlags::setType(MessageType type)
{
	mType = type;
}

MessageFormat MessageFlags::format() const
{
	return mFormat;
}

void MessageFlags::setFormat(MessageFormat format)
{
	mFormat = format;
}

QChar MessageFlags::typeAsChar() const
{
	switch(mType)
	{
		case INTERNAL: return INTERNAL_CHAR;
		case PRIVATE: return PRIVATE_CHAR;
		default: throw runtime_error("Unknown type flag: " + mType);
	}
}

MessageType MessageFlags::typeFromChar(QChar typeCharacter) const
{
	if(typeCharacter == INTERNAL_CHAR) return INTERNAL;
	if(typeCharacter == PRIVATE_CHAR) return PRIVATE;
	throw runtime_error("Unknown type flag: " + typeCharacter.toLatin1());
}

QChar MessageFlags::formatAsChar() const
{
	switch(mFormat)
	{
		case RAW: return RAW_CHAR;
		case UTF8: return UTF8_CHAR;
		default: throw runtime_error("Unknown format flag: " + mFormat);
	}
}

MessageFormat MessageFlags::formatFromChar(QChar formatCharacter) const
{
	if(formatCharacter == RAW_CHAR) return RAW;
	if(formatCharacter == UTF8_CHAR) return UTF8;
	throw runtime_error("Unknown type flag: " + formatCharacter.toLatin1());
}

QString MessageFlags::flagString() const
{
	QString flagString;
	flagString += "Type: ";
	flagString += typeAsChar();
	flagString += " Format: ";
	flagString += formatAsChar();
	return flagString;
}

QByteArray MessageFlags::flagBytes() const
{
	QByteArray flagBytes;
	QDataStream flagByteStream(&flagBytes, QIODevice::WriteOnly);
	flagByteStream << typeAsChar();
	flagByteStream << formatAsChar();
	return flagBytes;
}
