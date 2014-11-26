#ifndef NETWORKSTREAM_HPP
#define NETWORKSTREAM_HPP

#include <QObject>
#include <QAbstractSocket>
#include <QByteArray>
#include <QString>
#include <QTextCodec>

class NetworkStream : public QObject
{
private:
	Q_OBJECT

	QAbstractSocket* mSocket;

	QByteArray mMessageBuffer;
	qint32 mMessageSize;
	bool mProcessingMessage;

	void readMessageSize();
	void readAvailableData();

public:
	explicit NetworkStream(QAbstractSocket* socket, QObject *parent = 0);
	void hook();
	void unhook();
signals:
	void messageReady(QByteArray m);
public slots:
	void dataRecieved();
	void writeMessage(QString m);
	void writeMessage(QByteArray messageBytes);
};

#endif // NETWORKSTREAM_HPP
