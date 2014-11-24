#include "NetUtil.hpp"

#include <iostream>
using namespace std;

QByteArray readBytesFromSocket(QTcpSocket* socket, qint64 maxSize)
{
	QByteArray messageBuffer(maxSize, '!');
	quint64 bytesRead = socket->read(messageBuffer, maxSize);
	cout << "Read " << messageBytesRead << " bytes" << endl;
	messageBuffer.resize(bytesRead);

	return messageBuffer;
}

