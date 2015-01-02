#ifndef REMOTESTRINGRESOURCE_HPP
#define REMOTESTRINGRESOURCE_HPP

#include "RemoteResource.hpp"
#include <QString>

class RemoteStringResource : public RemoteResource
{
public:
	RemoteStringResource(QString resourceName, QObject* parent=0);
	~RemoteStringResource();
	QString string() const;
};

#endif // REMOTESTRINGRESOURCE_HPP
