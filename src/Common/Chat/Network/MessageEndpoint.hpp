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
	NOT_IDENTIFIED,
	IDENTITY_REQUESTED,
	IDENTITY_SENT,
	IDENTIFIED,
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
protected:
	const QString IDENTITY_REQUEST_STRING = "IDENTIFY";
	const QString IDENTIFIED_STRING = "IDENTIFIED";
	const QString IDENTITY_UPDATE_AVAILABLE_STRING = "IDENTITY_UPDATE_AVAILABLE";
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
	IdentState mLocalIdentState = NOT_IDENTIFIED;
	IdentState mRemoteIdentState = NOT_IDENTIFIED;

	PixmapState mLocalPixmapState = PIXMAP_NOT_SENT;
	PixmapState mRemotePixmapState = PIXMAP_NOT_SENT;

	QPixmap mRemotePixmap;

	virtual void handleInternalMessage(ChatMessage* m);

	void handleIndentityMessage(ChatMessage* request);
	void sendIdentity();
	void identityRecieved(QString remoteName);

	void handlePixmapExchangeMessage(ChatMessage* pixmapMessage);
	void recievePixmap(ChatMessage* m);
	void sendPixmap();

	void writeInternalMessageString(QString messageString, MessageType type);
	void writeInternalMessageBytes(QByteArray messageBytes, MessageType type, MessageFormat format);

	void sendIdentError();

	QString identityStateString();
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
	void connectionFailed(ConnectionError ce);
	void remoteIdentified();
	void identificationSuccessful();
	void identificationFailed(ConnectionError connectionError);
	void unidentifiedClientSentMessage();
	void remotePixmapChanged();
	void remoteNameChanged();
protected slots:
	void readChatMessage(QByteArray messageBytes);
	void handleSocketError(QAbstractSocket::SocketError error);
public slots:
	void writeChatMessage(ChatMessage* m);
	void setRemoteName(QString name);
	void requestIdentity();
	void notifyRemoteAboutIdentityUpdate();
	void requestPixmap();
	void notifyRemoteAboutPixmapUpdate();
};

#endif // MESSAGEENDPOINT_HPP
