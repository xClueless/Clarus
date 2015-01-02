#ifndef ENDPOINTMANAGER_HPP
#define ENDPOINTMANAGER_HPP

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QUdpSocket>
#include <stdint.h>
#include <QString>
#include <QMap>
#include <QPixmap>
#include "../ChatMessage.hpp"
#include "MessageClient.hpp"
#include "MessageServer.hpp"
#include "../ConnectionError.hpp"

class EndpointManager : public QObject
{
private:
		Q_OBJECT
		const QString BROADCAST_CONNECT_STRING = "CONNECT_BACK";

		QTcpServer* mServerSocket;
		QUdpSocket* mBroadcastSocket;
		uint32_t mPort;

		QList<MessageEndpoint*> mEndpoints;
public:
		explicit EndpointManager(uint32_t port, QObject *parent = 0);
		bool endpointIsConnected(QString remoteName);
		bool isLocalAddress(QString remoteName);
signals:
		void endpointConnected(MessageEndpoint* endpoint);
		void connectionFailed(MessageEndpoint* endpoint, ConnectionError connectionError);
protected slots:
		void handleConnectionFailure(ConnectionError connectionError);
		void endpointInitialisedConnection();
		void processDatagrams();
public slots:
		void start();
		void connectToEndpoint(QString serverHostname);
		void sendBroadcast();
};

#endif // ENDPOINTMANAGER_HPP
