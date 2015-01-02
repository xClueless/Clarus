#include "RemotePixmap.hpp"

#include <QDebug>

RemotePixmapResource::RemotePixmapResource(QString resourceName, QObject* parent)
	: RemoteResource(resourceName, parent)
{
}
void RemotePixmapResource::recieveResource(ResourceMessage* m)
{
	qDebug() << "[RemotePixmap] Loading remote pixmap.";

	RemoteResource::recieveResource(m);
	if(mPixmap.loadFromData(mResourceData))
	{

	}
	else
	{
		qDebug() << "[RemotePixmap] Failed to load remote pixmap.";
		mResourceState = NOT_AVAILABLE;
		sendStateUpdate();
	}
}
const QPixmap& RemotePixmapResource::pixmap()
{
	return mPixmap;
}

