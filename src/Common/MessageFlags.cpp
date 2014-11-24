#include "MessageFlags.hpp"

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
