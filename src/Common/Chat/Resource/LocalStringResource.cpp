#include "LocalStringResource.hpp"

LocalStringResource::LocalStringResource(QString resourceName, QString resourceValue,
										 QObject* parent)
	: LocalResource(resourceName, parent)
{
	setString(resourceValue);
}

LocalStringResource::~LocalStringResource()
{
}

void LocalStringResource::setString(const QString& s)
{
	setData(s.toUtf8());
}

QString LocalStringResource::string()
{
	return QString::fromUtf8(mResourceData);
}

