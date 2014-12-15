#ifndef MESSAGECLIENT_HPP
#define MESSAGECLIENT_HPP

#include "MessageEndpoint.hpp"

class MessageClient : public MessageEndpoint
{
private:
	Q_OBJECT
public:
	MessageClient(EndpointManager* clientManager, QTcpSocket* socket, QObject* parent=0);
public slots:
	void handleInternalMessage(ChatMessage* request);
	void connectToServer(QString clientHostname, quint16 mPort);
};

#endif // MESSAGESERVER_HPP
