#ifndef MESSAGESERVER_HPP
#define MESSAGESERVER_HPP

#include "MessageEndpoint.hpp"
#include <QMap>

class MessageServer : public MessageEndpoint
{
private:
	Q_OBJECT
public:
	explicit MessageServer(EndpointManager* clientManager, QTcpSocket* socket, QObject* parent = 0);
public slots:
	void handleInternalMessage(ChatMessage* m);
};

#endif // MESSAGECLIENT_HPP
