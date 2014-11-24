#include "NetUtil.hpp"

#include <iostream>
using namespace std;

QByteArray readBytesFromSocket(QTcpSocket* socket, qint64 maxSize)
{
	QByteArray messageBuffer(maxSize, '!');
	quint64 bytesRead = socket->read(messageBuffer.data(), maxSize);
	cout << "Read " << bytesRead << " bytes" << endl;
	messageBuffer.resize(bytesRead);

	return messageBuffer;
}

