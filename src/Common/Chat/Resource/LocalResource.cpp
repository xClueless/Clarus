#include "LocalResource.hpp"

#include "ResourceMessage.hpp"
#include <QDebug>

void LocalResource::notifyStale()
{
	setState(STALE);
	sendStateUpdate();
	setState(FRESH);
}

LocalResource::LocalResource(QString resourceName, QObject* parent) : ChatResource(resourceName, parent)
{
}
LocalResource::~LocalResource()
{
}

void LocalResource::setResourceBytes(QByteArray bytes)
{
	setData(bytes);
	notifyStale();
}
void LocalResource::setResourceUTF8(QString utf8)
{
	setDataUtf8(utf8);
}

void LocalResource::handleResourceMessage(ResourceMessage* m)
{
	if(m->resourceOperation() == READY_TO_RECIEVE_DATA)
	{
		sendDataUpdate();
	}
	else
	{
		qDebug() << "[LocalResource] Unhandled error case." << endl;
	}
}
