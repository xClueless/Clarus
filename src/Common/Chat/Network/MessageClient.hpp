#ifndef MESSAGECLIENT_HPP
#define MESSAGECLIENT_HPP

#include "MessageEndpoint.hpp"

class MessageClient : public MessageEndpoint
{
private:
	Q_OBJECT
	void handleIdentityMessage(ChatMessage* request);
	void identityRecieved(ChatMessage* m);
public:
	MessageClient(ClientManager* clientManager, QTcpSocket* socket, QObject* parent=0);
public slots:
	void sendName();
	void processInternalMessage(ChatMessage* request);
	void connectToServer(QString clientHostname, quint16 mPort);
signals:
	void identificationSuccesful();
	void identificationFailed(IdentFailure identFail);
};

#endif // MESSAGESERVER_HPP
