#ifndef MESSAGEENDPOINT_HPP
#define MESSAGEENDPOINT_HPP

#include <QObject>
#include <QString>
#include <QTcpSocket>
#include <QMap>
#include <QPixmap>
#include "NetworkStream.hpp"
#include "../ChatMessage.hpp"
#include "../ConnectionError.hpp"
#include "../Identity.hpp"
#include "../Resource/RemoteResource.hpp"

class EndpointManager;

class MessageEndpoint : public QObject
{
private:
	Q_OBJECT
protected:
	EndpointManager* mEndpointManager;
	QString mRemoteName = "UNKNOWN_REMOTE";
	QTcpSocket* mSocket;
	NetworkStream mNetworkStream;
	QMap<QString, ChatResource*> mSharedResources;

	RemoteResource* mRemoteIdentity;

	QPixmap mRemotePixmap;
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
};

#endif // MESSAGEENDPOINT_HPP
