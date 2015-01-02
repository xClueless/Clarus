#include "MessageHandler.hpp"

MessageHandler::MessageHandler(QString handlerName, QObject* parent)
	: QObject(parent), mHandlerName(handlerName)
{
}

MessageHandler::~MessageHandler()
{
}

QString MessageHandler::messageHandlerName()
{
	return mHandlerName;
}

