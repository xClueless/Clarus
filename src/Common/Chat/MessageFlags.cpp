#include "MessageFlags.hpp"

#include <stdexcept>
#include <iostream>
#include <QDataStream>

using namespace std;

MessageFlags::MessageFlags()
{
}
MessageFlags::MessageFlags(MessageType type, MessageFormat format) : mType(type), mFormat(format)
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
		case MessageType::IDENTIFICATION: return INDENTIFICATION_CHAR;
		case MessageType::PIXMAP_EXCHANGE: return ENDPOINT_PIXMAP_EXCHANGE_CHAR;
		case MessageType::PRIVATE: return PRIVATE_CHAR;
		case MessageType::PROTOCOL_ERROR: return PROTOCOL_ERROR_CHAR;
		default: throw runtime_error("Unknown type flag");
	}
}

MessageType MessageFlags::typeFromChar(QChar typeCharacter) const
{
	if(typeCharacter == INDENTIFICATION_CHAR) return MessageType::IDENTIFICATION;
	if(typeCharacter == ENDPOINT_PIXMAP_EXCHANGE_CHAR) return MessageType::PIXMAP_EXCHANGE;
	if(typeCharacter == PRIVATE_CHAR) return MessageType::PRIVATE;
	if(typeCharacter == PROTOCOL_ERROR_CHAR) return MessageType::PROTOCOL_ERROR;
	throw runtime_error("Unknown type flag: " + typeCharacter.toLatin1());
}

QChar MessageFlags::formatAsChar() const
{
	switch(mFormat)
	{
		case MessageFormat::RAW: return RAW_CHAR;
		case MessageFormat::UTF8: return UTF8_CHAR;
		default: throw runtime_error("Unknown format flag");
	}
}

MessageFormat MessageFlags::formatFromChar(QChar formatCharacter) const
{
	if(formatCharacter == RAW_CHAR) return MessageFormat::RAW;
	if(formatCharacter == UTF8_CHAR) return MessageFormat::UTF8;
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
