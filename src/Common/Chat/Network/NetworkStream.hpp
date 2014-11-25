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
	QTextCodec* mTextCodec;

	QByteArray mMessageBuffer;
	qint32 mMessageSize;
	bool mProcessingMessage;

	void readMessageSize();
	void readAvailableData();

public:
	explicit NetworkStream(QAbstractSocket* socket, QObject *parent = 0);
	void hook();
	void unhook();
	void writeMessage(QString m);
signals:
	void messageReady(QString m);
public slots:
	void dataRecieved();
};

#endif // NETWORKSTREAM_HPP
