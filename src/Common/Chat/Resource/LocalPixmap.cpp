#include "LocalPixmap.hpp"

#include <QBuffer>
#include <QDebug>

QByteArray LocalPixmapResource::toPngArray(QPixmap p)
{
	qDebug() << "toPngArray(QPixmap)";
	QByteArray pixmapArray;
	QBuffer pixmapBuffer(&pixmapArray);
	pixmapBuffer.open(QIODevice::WriteOnly);
	p.save(&pixmapBuffer, "PNG");
	return pixmapArray;
}

LocalPixmapResource::LocalPixmapResource(QString resourceName, QObject* parent)
	: LocalResource(resourceName, parent)
{
}
LocalPixmapResource::~LocalPixmapResource()
{

}

const QPixmap& LocalPixmapResource::pixmap()
{
	return mPixmap;
}

void LocalPixmapResource::setPixmap(QPixmap pixmap)
{
	mPixmap = pixmap;
	setData(toPngArray(mPixmap));
}

void LocalPixmapResource::loadPixmap(QString path)
{
	mPixmap.load(path);
	setData(toPngArray(mPixmap));
}

