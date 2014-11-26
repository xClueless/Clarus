#ifndef MESSAGEENDPOINT_HPP
#define MESSAGEENDPOINT_HPP

#include <QObject>
#include <QString>
#include <QTcpSocket>
#include <QPixmap>
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

enum PixmapState
{
	PIXMAP_NOT_SENT,
	PIXMAP_REQUESTED,
	PIXMAP_SENT,
	PIXMAP_RECIEVED,
	PIXMAP_UPDATE_AVAILABLE,
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

	const QString PIXMAP_REQUEST_STRING = "SEND_PIXMAP";
	const QString PIXMAP_RECIEVED_STRING = "RECIEVED_PIXMAP";
	const QString PIXMAP_UPDATE_AVAILABLE_STRING = "PIXMAP_UPDATE_AVAILABLE";

	ClientManager* mClientManager;
	QString mRemoteName = "UNKNOWN_REMOTE";
	QTcpSocket* mSocket;
	NetworkStream mNetworkStream;
	IdentState mIdentState = NOT_IDENTIFIED;

	PixmapState mLocalPixmapState = PIXMAP_NOT_SENT;
	PixmapState mRemotePixmapState = PIXMAP_NOT_SENT;

	QPixmap mRemotePixmap;

	void writeInternalMessageString(QString messageString, MessageType type);
	void writeInternalMessageBytes(QByteArray messageBytes, MessageType type, MessageFormat format);
	void handlePixmapMessage(ChatMessage* pixmapMessage);

	QString identStateString();
	QString pixmapStateString();
public:
	explicit MessageEndpoint(ClientManager* clientManager, QTcpSocket* socket, QObject *parent = 0);
	virtual ~MessageEndpoint();

	QString remoteName();
	QTcpSocket* socket();
	QPixmap& remotePixmap();

	bool operator ==(MessageEndpoint* endpoint);
	bool operator !=(MessageEndpoint* endpoint);
signals:
	void messageReady(ChatMessage* m);
	void internalMessageReady(ChatMessage* m);
	void remoteNameChanged();
	void connectionFailed(ConnectionError ce);
	void remotePixmapChanged();
public slots:
	void readChatMessage(QByteArray messageBytes);
	void writeChatMessage(ChatMessage* m);
	virtual void processInternalMessage(ChatMessage* m);
	void setRemoteName(QString name);
	void handleSocketError(QAbstractSocket::SocketError error);

	void requestPixmap();
	void sendPixmap();
	void notifyRemoteAboutPixmapUpdate();
	void recievePixmap(ChatMessage* m);
};

#endif // MESSAGEENDPOINT_HPP
