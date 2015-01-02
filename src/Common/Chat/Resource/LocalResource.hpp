#ifndef LOCALRESOURCE_HPP
#define LOCALRESOURCE_HPP

#include "ChatResource.hpp"

class LocalResource : public ChatResource
{
private:
	void notifyStale();
public:
	LocalResource(QString resourceName, QObject* parent=0);
	~LocalResource();

public slots:
	void setData(QByteArray bytes);
public slots:
	void handleResourceMessage(ResourceMessage* m);
};

#endif // LOCALRESOURCE_HPP
