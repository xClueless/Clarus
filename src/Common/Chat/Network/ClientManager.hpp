#ifndef CLIENTMANAGER_HPP
#define CLIENTMANAGER_HPP

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QUdpSocket>
#include <stdint.h>
#include <QString>
#include <QMap>
#include "../ChatMessage.hpp"
#include "MessageClient.hpp"
#include "MessageServer.hpp"
#include "../ConnectionError.hpp"

class ClientManager : public QObject
{
private:
	Q_OBJECT

	QTcpServer* mServerSocket;
	QUdpSocket* mBroadcastSocket;

	QString mLocalName;
	uint32_t mPort;
	QList<MessageEndpoint*> mIdentifiedEndpoints;
	QList<MessageServer*> mServersThatNeedIdentification;
	QList<MessageClient*> mClientsThatNeedToIdentify;

	void addEnpointAsIdentified(MessageEndpoint* endpoint);
public:
	explicit ClientManager(QString name, uint32_t port, QObject *parent = 0);
	QList<MessageEndpoint*> identifiedEndpoints();
	QString localName();
	bool endpointIsConnected(QString remoteName);
signals:
	void endpointIdentified(MessageEndpoint* endpoint);
	void failedToConnectToEndpoint(MessageEndpoint* endpoint, ConnectionError connectionError);
	void localNameChanged();
protected slots:
	void serverIdentifiedUs();
	void serverFailedToIdentifyUs(ConnectionError connectionError);
	void clientIdentified();
	void clientFailedToIdentify(ConnectionError connectionError);
public slots:
	void start();
	void newClientConnected();
	void connectToServer(QString serverHostname);
	void setLocalName(QString name);
	void sendBroadcast();
	void processDatagrams();
};

#endif // CLIENTMANAGER_HPP
