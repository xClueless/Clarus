#ifndef MESSAGECLIENT_HPP
#define MESSAGECLIENT_HPP

#include <QObject>
#include <QTcpSocket>
#include "ChatMessage.hpp"
#include "NetworkStream.hpp"
#include <QQueue>

class MessageClient : public QObject
{
private:
	Q_OBJECT

	QTcpSocket* mSocket;
	QString mRemoteClientName;
	QString mLocalClientName;
	QQueue<ChatMessage*> mInternalMessages;
	NetworkStream mClientStream;

	void waitForInternalMessage();
	void writeInternalMessage(QString messageString);
	QString readInternalMessage();
public:
	explicit MessageClient(QTcpSocket* socket, QObject *parent = 0);
	QString remoteClientName();
signals:
	void messageReady(ChatMessage* m);
public slots:
	void identify(QStringList currentClientNames);
	void sendIdentify();
	void readChatMessage(QString messageString);
	void writeChatMessage(ChatMessage* m);
};

#endif // MESSAGECLIENT_HPP
