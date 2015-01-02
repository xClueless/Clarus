#ifndef LOCALSTRINGRESOURCE_HPP
#define LOCALSTRINGRESOURCE_HPP

#include "LocalResource.hpp"

class LocalStringResource : public LocalResource
{
public:
	LocalStringResource(QString resourceName, QString resourceValue="", QObject* parent=0);
	~LocalStringResource();
	void setString(const QString& s);
	QString string();
};

#endif // LOCALSTRINGRESOURCE_HPP
