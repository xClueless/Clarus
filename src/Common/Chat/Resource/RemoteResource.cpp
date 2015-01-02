#include "RemoteResource.hpp"
#include <QDebug>

RemoteResource::RemoteResource(QString resourceName, QObject* parent) : ChatResource(resourceName, parent)
{
	requestResource();
}

RemoteResource::~RemoteResource()
{

}
void RemoteResource::setAutoRefresh(bool autoRefresh)
{
	mAutoRefresh = autoRefresh;
}

void RemoteResource::requestResource()
{
	setState(READY_TO_RECIEVE_DATA);
	sendStateUpdate();
}

void RemoteResource::recieveResource(ResourceMessage* m)
{
	qDebug() << "[RemoteResource] Recieving resource from remote." << endl;
	QByteArray data = m->resourceData();
	setData(data);
	setState(FRESH);
}

void RemoteResource::handleResourceMessage(ResourceMessage* rm)
{
	if(rm->resourceOperation() == ResourceMessage::SENT_UPDATED_STATE && rm->resourceState() == UPDATE_AVAILABLE)
	{
		if(mAutoRefresh) requestResource();
		else setState(STALE);
	}
}
