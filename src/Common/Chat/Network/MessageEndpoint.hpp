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
#include "../Identity/RemoteIdentity.hpp"
#include "../Resource/RemoteResource.hpp"

class EndpointManager;

class MessageEndpoint : public QObject
{
private:
	Q_OBJECT
protected:
	EndpointManager* mEndpointManager;
	QTcpSocket* mSocket;
	NetworkStream mNetworkStream;

	//Resources
	RemoteIdentity mRemoteIdentity;
	QMap<QString, ChatResource*> mSharedResources;
public:
	explicit MessageEndpoint(EndpointManager* endpointManager, QTcpSocket* socket, QObject *parent = 0);
	virtual ~MessageEndpoint();

	RemoteIdentity* identity();
	QTcpSocket* socket();

	bool operator ==(MessageEndpoint* endpoint);
	bool operator !=(MessageEndpoint* endpoint);
signals:
	void messageReady(ChatMessage* m);
	void connectionFailed(ConnectionError ce);
protected slots:
	void readChatMessage(QByteArray messageBytes);
	void handleSocketError(QAbstractSocket::SocketError error);
	void handleProtocolError(QString error);
public slots:
	void writeChatMessage(ChatMessage* m);
};

#endif // MESSAGEENDPOINT_HPP
