#ifndef MESSAGEMARSHALLER_HPP
#define MESSAGEMARSHALLER_HPP

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <stdint.h>
#include <QString>
#include <QMap>
#include "ChatMessage.hpp"
#include "MessageClient.hpp"

class MessageMarshaller : public QObject
{
private:
	Q_OBJECT

	QTcpServer* mServerSocket;
	QString mName;
	uint32_t mPort;
	QMap<QString, MessageClient*> mClients;

	QString readNetworkMessage(QTcpSocket* socket);
	void writeNetworkMessage(QTcpSocket* socket, QString s);
public:
	explicit MessageMarshaller(QString name, uint32_t port, QObject *parent = 0);
	QMap<QString, MessageClient*> clients();

signals:
	void messageReady(ChatMessage* m);

public slots:
	void start();
	void newClientConnected();
	void sendMessage(ChatMessage* m);
	void connectToClient(QString clientHostname);
};

#endif // MESSAGEMARSHALLER_HPP
