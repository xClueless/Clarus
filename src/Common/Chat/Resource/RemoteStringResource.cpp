#include "RemoteStringResource.hpp"

RemoteStringResource::RemoteStringResource(QString resourceName,
										 QObject* parent)
	: RemoteResource(resourceName, parent)
{
}
RemoteStringResource::~RemoteStringResource()
{
}
QString RemoteStringResource::string() const
{
	return QString::fromUtf8(mResourceData);
}
