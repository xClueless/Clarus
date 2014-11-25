#include "MessageFlags.hpp"

#include <stdexcept>

using namespace std;

MessageFlags::MessageFlags(const MessageType type) : mType(type)
{
}

MessageFlags::MessageFlags(QString flagString)
{
	QChar type = flagString.at(0);
	if(type == INTERNAL_CHAR)
	{
		mType = INTERNAL;
	}
	else if(type == PRIVATE_CHAR)
	{
		mType = PRIVATE;
	}
	else
	{
		throw runtime_error("Failed to parse message. Type flag not set.");
	}
}

MessageType MessageFlags::type()
{
	return mType;
}

void MessageFlags::setType(MessageType type)
{
	mType = type;
}

QString MessageFlags::flagString()
{
	QString flagString;
	if(mType == INTERNAL)
	{
		flagString += INTERNAL_CHAR;
	}
	else if(mType == PRIVATE)
	{
		flagString += PRIVATE_CHAR;
	}
	return flagString;
}
