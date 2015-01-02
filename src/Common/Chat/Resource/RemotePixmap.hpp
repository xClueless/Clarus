#ifndef REMOTEPIXMAP_HPP
#define REMOTEPIXMAP_HPP

#include "RemoteResource.hpp"
#include <QPixmap>

class RemotePixmapResource : public RemoteResource
{
private:
	QPixmap mPixmap;
public:
	RemotePixmapResource(QString resourceName, QObject* parent=0);
	const QPixmap& pixmap();
	// RemoteResource interface
protected slots:
	void recieveResource(ResourceMessage* m);
};

#endif // REMOTEPIXMAP_HPP
