#ifndef NETWORKSTREAM_HPP
#define NETWORKSTREAM_HPP

#include <QObject>
#include <QTcpSocket>
#include <QByteArray>
#include <QString>

class NetworkStream : public QObject
{
private:
	Q_OBJECT

	QTcpSocket* mSocket;
	QTextCodec mTextCodec;

	QByteArray mMessageBuffer;
	qint32 mMessageSize;
	bool mProcessingMessage;

	void readMessageSize();
	void readAvailableData();

	void writeMessage(QString m);
public:
	explicit NetworkStream(QTcpSocket* socket, QObject *parent = 0);
	void hook();
	void unhook();
signals:
	void messageReady(QString m);
public slots:
	void dataRecieved();
};

#endif // NETWORKSTREAM_HPP
