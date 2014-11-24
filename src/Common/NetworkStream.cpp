#include "NetworkStream.hpp"

#include <QEventLoop>
#include <iostream>
#include <QTextCodec>
#include "NetUtil.hpp"

using namespace std;

NetworkStream::NetworkStream(QTcpSocket* socket, QObject *parent) :
	QObject(parent), mSocket(socket)
{
	mTextCodec = QTextCodec::codecForName("UTF-8");
	hook();
}

void NetworkStream::hook()
{
	connect(mSocket, SIGNAL(readyRead()), this, SLOT(dataRecieved()));
}

void NetworkStream::unhook()
{
	disconnect(mSocket, SIGNAL(readyRead()), this, NULL);
}

void NetworkStream::readMessageSize()
{
	QByteArray messageSizeArray = readBytesFromSocket(mSocket, sizeof(mMessageSize));
	mMessageSize = messageSizeArray.toInt();
}

void NetworkStream::readAvailableData()
{
	qint32 messageBytesRead = mMessageBuffer.size();
	qint32 sizeDiff = mMessageSize - messageBytesRead;

	mMessageBuffer += readBytesFromSocket(mSocket, sizeDiff);
}

void NetworkStream::writeMessage(QString message)
{
	QDataStream clientStream(mSocket);

	QByteArray messageArray = message.toUtf8();
	qint32 messageSize = messageArray.size();
	cout << "[NetworkStream] WRITE-MESSAGE '" << message.toStdString() << "' SIZE: " << messageSize << endl;

	clientStream << messageSize;
	clientStream << messageArray;
}

void NetworkStream::dataRecieved()
{
	if(mSocket->bytesAvailable() != 0)
	{
		if(!mProcessingMessage)
		{
			if(mSocket->bytesAvailable() >= sizeof(mMessageSize))
			{
				readMessageSize();
			}
		}
		else
		{
			readAvailableData();

			if(mMessageBuffer.size() == mMessageSize)
			{
				//The message is ready.
				mProcessingMessage = false;
				QString message = mTextCodec->toUnicode(mMessageBuffer);
				emit messageReady(message);
			}

			if(mSocket->bytesAvailable() != 0) //There may be residual bytes from a new message.
			{
				dataRecieved();
			}
		}
	}
}
