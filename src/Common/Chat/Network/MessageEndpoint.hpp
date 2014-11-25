#ifndef MESSAGEENDPOINT_HPP
#define MESSAGEENDPOINT_HPP

#include <QObject>
#include <QString>
#include <QTcpSocket>
#include "NetworkStream.hpp"
#include "../ChatMessage.hpp"
#include "../ConnectionError.hpp"

class ClientManager;

enum IdentState
{
	NOT_IDENTIFIED = 0,
	CLIENT_IDENTITY_REQUESTED = 1,
	CLIENT_IDENTITY_SENT = 2,
	CLIENT_IDENTIFIED = 3,
	SERVER_IDENTITY_REQUESTED = 4,
	SERVER_IDENTITY_SENT = 5,
	IDENTIFICATION_COMPLETE = 6
};

class MessageEndpoint : public QObject
{
private:
	Q_OBJECT
	void sendIdentError();
protected:
	const QString IDENTITY_REQUEST_STRING = "IDENTIFY";
	const QString IDENTIFIED_STRING = "IDENTIFIED";
	const QString SENT_EMPTY_NAME_STRING = "SENT_EMPTY_NAME";
	const QString UNSPECIFIED_IDENT_ERROR_STRING = "UNSPECIFIED_IDENT_ERROR";
	const QString REQUIRES_IDENTIFICATON_STRING = "REQUIRES_IDENTIFICATON";

	ClientManager* mClientManager;
	QString mRemoteName = "UNKNOWN_REMOTE";
	QTcpSocket* mSocket;
	NetworkStream mNetworkStream;
	IdentState mIdentState = NOT_IDENTIFIED;

	void writeInternalMessage(QString messageString);
	QString identStateString();
public:
	explicit MessageEndpoint(ClientManager* clientManager, QTcpSocket* socket, QObject *parent = 0);
	virtual ~MessageEndpoint();
	QString remoteName();

signals:
	void messageReady(ChatMessage* m);
	void internalMessageReady(ChatMessage* m);
	void remoteNameChanged();

public slots:
	void readChatMessage(QString messageString);
	void writeChatMessage(ChatMessage* m);
	virtual void processInternalMessage(ChatMessage* m);
	void setRemoteName(QString name);
};

#endif // MESSAGEENDPOINT_HPP
