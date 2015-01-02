#ifndef REMOTERESOURCE_HPP
#define REMOTERESOURCE_HPP

#include "../ChatMessage.hpp"
#include "ChatResource.hpp"
#include <QObject>

class RemoteResource : public ChatResource
{
private:
	bool mAutoRefresh = true;
public:
	RemoteResource(QString resourceName, QObject* parent=0);
	~RemoteResource();
	void setAutoRefresh(bool autoRefresh);
protected slots:
	virtual void recieveResource(ResourceMessage* m);
public slots:
	void handleResourceMessage(ResourceMessage* request);
	void requestResource();
signals:
	void recieveFailed(QString errorMessage);
};

#endif // REMOTERESOURCE_HPP
