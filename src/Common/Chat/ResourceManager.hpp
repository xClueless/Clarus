#ifndef RESOURCEMANAGER_HPP
#define RESOURCEMANAGER_HPP

#include <QObject>
#include <QMap>
#include "Resource/LocalResource.hpp"

class ResourceManager : public QObject
{
private:
	Q_OBJECT
	QMap<QString, LocalResource*> mLocalResources;
public:
	explicit ResourceManager(QObject *parent = 0);
	~ResourceManager();

	void addLocalResource(LocalResource* resource);
	void removeLocalResource(const QString& resourceName);
	void deleteLocalResource(const QString& resourceName);
	LocalResource* localResource(const QString& resourceName);

signals:

public slots:
};

#endif // RESOURCEMANAGER_HPP
