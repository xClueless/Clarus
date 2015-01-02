#include "LocalResource.hpp"

#include "ResourceMessage.hpp"
#include <QDebug>
#include <QBuffer>

void LocalResource::notifyStale()
{
	qDebug() << "[LocalResource " << mResourceName << "] Sending stale notification";
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

void LocalResource::setData(QByteArray bytes)
{
	ChatResource::setData(bytes);
	notifyStale();
}
void LocalResource::handleResourceMessage(ResourceMessage* m)
{
	if(m->resourceOperation() == ResourceMessage::SENT_UPDATED_STATE)
	{
		qDebug() << "[LocalResource " << mResourceName << "] Remote sent a new state";

		if(m->resourceState() == READY_TO_RECIEVE_DATA)
		{
			qDebug() << "[LocalResource " << mResourceName << "] Sending remote new data";
			sendDataUpdate();
		}
	}
	else
	{
		qDebug() << "[LocalResource] Unhandled error case." << endl;
	}
}
