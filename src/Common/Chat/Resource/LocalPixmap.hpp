#ifndef LOCALPIXMAP_HPP
#define LOCALPIXMAP_HPP

#include "LocalResource.hpp"
#include <QPixmap>

class LocalPixmapResource : public LocalResource
{
private:
	QPixmap mPixmap;
	QByteArray toPngArray(QPixmap p);
public:
	LocalPixmapResource(QString resourceName, QObject* parent=0);
	~LocalPixmapResource();
	const QPixmap& pixmap();
	void setPixmap(QPixmap pixmap);
	void loadPixmap(QString path);
};

#endif // LOCALPIXMAP_HPP
