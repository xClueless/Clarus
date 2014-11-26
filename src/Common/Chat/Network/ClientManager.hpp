#ifndef CLIENTMANAGER_HPP
#define CLIENTMANAGER_HPP

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

class ClientManager : public QObject
{
private:
	Q_OBJECT
	const QString BROADCAST_CONNECT_STRING = "CONNECT_BACK";
	const QString DEFAULT_PIXMAP_URL = ":/images/default-contact-icon.png";

	QTcpServer* mServerSocket;
	QUdpSocket* mBroadcastSocket;
	uint32_t mPort;

	QString mLocalName;
	QPixmap mLocalPixmap;

	QList<MessageEndpoint*> mIdentifiedEndpoints;
	QList<MessageServer*> mServersThatNeedIdentification;
	QList<MessageClient*> mClientsThatNeedToIdentify;

	void addEnpointAsIdentified(MessageEndpoint* endpoint);
public:
	explicit ClientManager(QString name, uint32_t port, QObject *parent = 0);
	QList<MessageEndpoint*> identifiedEndpoints();
	bool endpointIsConnected(QString remoteName);
	bool isLocalAddress(QString remoteName);

	QString localName();
	QPixmap& localPixmap();

signals:
	void endpointIdentified(MessageEndpoint* endpoint);
	void failedToConnectToEndpoint(MessageEndpoint* endpoint, ConnectionError connectionError);
	void localNameChanged();
	void localPixmapChanged();
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
	void loadLocalPixmap(QString pixmapURL);
	void sendBroadcast();
	void processDatagrams();
};

#endif // CLIENTMANAGER_HPP
