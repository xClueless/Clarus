#include "NetUtil.hpp"

#include <iostream>
using namespace std;

QByteArray readBytesFromSocket(QTcpSocket* socket, qint64 maxSize)
{
	qint64 bufferSize = maxSize;
	qint64 bytesAvailable = socket->bytesAvailable();

	if(bytesAvailable < maxSize)
	{
		bufferSize = bytesAvailable;
	}

	QByteArray messageBuffer(bufferSize, '!');
	quint64 bytesRead = socket->read(messageBuffer.data(), bufferSize);
//	cout << "[NetUtil] Read " << bytesRead << " bytes" << endl;
	messageBuffer.resize(bytesRead);

	return messageBuffer;
}

