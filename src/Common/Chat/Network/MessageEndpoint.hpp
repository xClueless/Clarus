#ifndef MESSAGEENDPOINT_HPP
#define MESSAGEENDPOINT_HPP

#include <QObject>
#include <QString>
#include <QTcpSocket>
#include <QPixmap>
#include "NetworkStream.hpp"
#include "../ChatMessage.hpp"
#include "../ConnectionError.hpp"
#include "../Identity.hpp"
#include "IdentityExchange.hpp"

class EndpointManager;
class IdentityExchange;

class MessageEndpoint : public QObject
{
private:
	Q_OBJECT
protected:
	EndpointManager* mEndpointManager;
	QString mRemoteName = "UNKNOWN_REMOTE";
	QTcpSocket* mSocket;
	NetworkStream mNetworkStream;

	IdentityExchange* mIdentityExchange;

	QPixmap mRemotePixmap;

	virtual void handleInternalMessage(ChatMessage* m);

	void sendIdentity();
	void identityRecieved(QString remoteName);

	void handlePixmapExchangeMessage(ChatMessage* pixmapMessage);
	void recievePixmap(ChatMessage* m);
	void sendPixmap();
	void sendIdentError();

	QString pixmapStateString();
public:
	explicit MessageEndpoint(EndpointManager* endpointManager, QTcpSocket* socket, QObject *parent = 0);
	virtual ~MessageEndpoint();

	QString remoteName();
	QTcpSocket* socket();
	QPixmap& remotePixmap();

	bool operator ==(MessageEndpoint* endpoint);
	bool operator !=(MessageEndpoint* endpoint);
signals:
	void messageReady(ChatMessage* m);
	void connectionFailed(ConnectionError ce);
	void remotePixmapChanged();
protected slots:
	void readChatMessage(QByteArray messageBytes);
	void handleSocketError(QAbstractSocket::SocketError error);
	void handleProtocolError(QString error);
public slots:
	void writeChatMessage(ChatMessage* m);
	void requestPixmap();
	void notifyRemoteAboutPixmapUpdate();
	void writeInternalMessageString(QString messageString, MessageType type);
	void writeInternalMessageBytes(QByteArray messageBytes, MessageType type, MessageFormat format);
};

#endif // MESSAGEENDPOINT_HPP
