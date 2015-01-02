#include "ResourceManager.hpp"
#include <QDebug>

ResourceManager::ResourceManager(QObject *parent) : QObject(parent)
{
}

ResourceManager::~ResourceManager()
{
}

void ResourceManager::addLocalResource(LocalResource* resource)
{
	if(mLocalResources.contains(resource->resourceName()))
	{
		qDebug() << "[ResourceManager] Error, resource already exists: " << resource->messageHandlerName();
	}
	else
	{
		mLocalResources[resource->resourceName()] = resource;
	}
}

void ResourceManager::removeLocalResource(const QString& resourceName)
{
	if(mLocalResources.contains(resourceName))
	{
		mLocalResources.remove(resourceName);
	}
	else
	{
		qDebug() << "[ResourceManager] Error, resource does not exist!";
	}
}

void ResourceManager::deleteLocalResource(const QString& resourceName)
{
	if(mLocalResources.contains(resourceName))
	{
		delete mLocalResources[resourceName];
	}
	else
	{
		qDebug() << "[ResourceManager] Error, resource does not exist!";
	}
}

LocalResource* ResourceManager::localResource(const QString& resourceName)
{
	return mLocalResources[resourceName];
}

