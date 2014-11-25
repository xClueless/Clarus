#ifndef CLIENTMANAGER_HPP
#define CLIENTMANAGER_HPP

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <stdint.h>
#include <QString>
#include <QMap>
#include "../ChatMessage.hpp"
#include "MessageClient.hpp"
#include "MessageServer.hpp"

class ClientManager : public QObject
{
private:
	Q_OBJECT

	QTcpServer* mServerSocket;
	QString mName;
	uint32_t mPort;
	QList<MessageEndpoint*> mIdentifiedEndpoints;
	QList<MessageServer*> mServersThatNeedIdentification;
	QList<MessageClient*> mClientsThatNeedToIdentify;

	void addEnpointAsIdentified(MessageEndpoint* endpoint);
public:
	explicit ClientManager(QString name, uint32_t port, QObject *parent = 0);
	QList<MessageEndpoint*> identifiedEndpoints();
	QString localName();
signals:
	void endpointIdentified(MessageEndpoint* endpoint);

protected slots:
	void serverIdentifiedUs();
	void serverFailedToIdentifyUs(IdentFailure identFail);
	void clientIdentified();
	void clientFailedToIdentify(IdentFailure identFail);
public slots:
	void start();
	void newClientConnected();
	void connectToServer(QString serverHostname);
};

#endif // CLIENTMANAGER_HPP
