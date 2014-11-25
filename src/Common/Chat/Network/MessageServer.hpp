#ifndef MESSAGESERVER_HPP
#define MESSAGESERVER_HPP

#include "MessageEndpoint.hpp"
#include <QMap>

class MessageServer : public MessageEndpoint
{
private:
	Q_OBJECT
	void identityRecieved(ChatMessage* m);
	void sendIdentity();
	void handleIndentityMessage(ChatMessage* request);
public:
	explicit MessageServer(ClientManager* clientManager, QTcpSocket* socket, QObject* parent = 0);
signals:
	void unidentifiedClientSentMessage();
	void identificationSuccesful();
	void identificationFailed(IdentFailure identFail);
public slots:
	void processInternalMessage(ChatMessage* m);
	void requestIdentification();
};

#endif // MESSAGECLIENT_HPP
