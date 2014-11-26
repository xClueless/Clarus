#include "MessageFlags.hpp"

#include <stdexcept>
#include <iostream>
#include <QDataStream>

using namespace std;

MessageFlags::MessageFlags()
{
}
MessageFlags::MessageFlags(const MessageType type, quint16 endpointListSize) : mType(type), mEndpointListSize(endpointListSize)
{
}

MessageFlags::MessageFlags(QByteArray flagBytes)
{
	if(flagBytes.size() != FLAG_SECTION_BYTES)
	{
		throw runtime_error("Failed to parse flags. Flag byte array does not contain the correct number of bytes.");
	}
	QDataStream flagByteStream(flagBytes);

	char typeChar;
	flagByteStream.readRawData(&typeChar, 1);
	mType = typeFromChar(typeChar);

	flagByteStream >> mEndpointListSize;
}

MessageType MessageFlags::type() const
{
	return mType;
}

void MessageFlags::setType(MessageType type)
{
	mType = type;
}

quint16 MessageFlags::endpointListSize() const
{
	return mEndpointListSize;
}

void MessageFlags::setEndpointListSize(quint16 listSize)
{
	mEndpointListSize = listSize;
}

char MessageFlags::typeAsChar() const
{
	switch(mType)
	{
		case INTERNAL: return INTERNAL_CHAR;
		case PRIVATE: return PRIVATE_CHAR;
		case RAW: return RAW_CHAR;
		default: throw runtime_error("Unknown type flag: " + mType);
	}
}

MessageType MessageFlags::typeFromChar(char typeCharacter) const
{
	switch(typeCharacter)
	{
		case INTERNAL_CHAR: return INTERNAL;
		case PRIVATE_CHAR: return PRIVATE;
		case RAW_CHAR: return RAW;
		default: throw runtime_error("Unknown type flag: " + typeCharacter);
	}
}

QString MessageFlags::flagString() const
{
	QString flagString;
	flagString += "Type: ";
	flagString += typeAsChar();
	flagString += " EndpointListSize: ";
	flagString += QString::number(mEndpointListSize);
	return flagString;
}

QByteArray MessageFlags::flagBytes() const
{
	QByteArray flagBytes;
	QDataStream flagByteStream(&flagBytes, QIODevice::WriteOnly);
	flagByteStream << typeAsChar();
	flagByteStream << mEndpointListSize;
	return flagBytes;
}
