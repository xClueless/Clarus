#include "NetworkStream.hpp"

#include <QEventLoop>
#include <iostream>
#include <QTextCodec>
#include "NetUtil.hpp"

using namespace std;

NetworkStream::NetworkStream(QAbstractSocket* socket, QObject *parent) :
	QObject(parent), mSocket(socket)
{
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
	QDataStream byteStream(&messageSizeArray, QIODevice::ReadOnly);
	byteStream >> mMessageSize;
//	cout << "[NetworkStream] Message has size: " << mMessageSize << endl;
}

void NetworkStream::readAvailableData()
{
	qint32 messageBytesRead = mMessageBuffer.size();
	qint32 sizeDiff = mMessageSize - messageBytesRead;

	if(mMessageSize <= 0)
	{
		while(true)
		{
			cerr << "Message size is not positive. This is a bug." << endl;
			exit(-1);
		}
	}

//	cout << "readAvailableData() Reading max bytes " << sizeDiff << " bytes. Message size: " << mMessageSize << endl;
	if(sizeDiff <= 0)
	{
		while(true)
		{
			cerr << "Size diff is not positive. This is a bug." << endl;
		}
	}

	QByteArray newData = readBytesFromSocket(mSocket, sizeDiff);
	mMessageBuffer += newData;
	cout << "[NetworkStream] Read " << newData.size() << " bytes of new data." << endl;
}

void NetworkStream::writeMessage(QString message)
{
	QByteArray messageBytes = message.toUtf8();
	cout << "[NetworkStream] WRITE-MESSAGE '" << message.toStdString() << "' SIZE: " << message.length() << endl;
	writeMessage(messageBytes);
}

void NetworkStream::writeMessage(QByteArray messageBytes)
{
	qint32 messageSize = messageBytes.size();

	QByteArray sizeArray;
	QDataStream byteStream(&sizeArray, QIODevice::WriteOnly);
	byteStream << messageSize;

	mSocket->write(sizeArray);
//	cout << "[NetworkStream] Sent message size." << endl;

	mSocket->write(messageBytes);
	//	cout << "[NetworkStream] Sent message data." << endl;
}

void NetworkStream::dataRecieved()
{
//	cout << "[NetworkStream] dataRecieved()" << endl;
	if(mSocket->bytesAvailable() > 0)
	{
		if(!mProcessingMessage)
		{
			if(mSocket->bytesAvailable() >= sizeof(mMessageSize))
			{
//				cout << "[NetworkStream] Processing new message" << endl;
				readMessageSize();
				mProcessingMessage = true;
			}
		}
		//We need to re-evaluate whether or not there are bytes available now and whether or not we're processing a message.
		if(mSocket->bytesAvailable() > 0 && mProcessingMessage)
		{
//			cout << "[NetworkStream] More data recieved. Reading it now." << endl;
			readAvailableData();

			if(mMessageBuffer.size() == mMessageSize)
			{
				//The message is ready.
				mProcessingMessage = false;
				emit messageReady(mMessageBuffer);
				mMessageBuffer.clear();
//				cout << "[NetworkStream] READ-MESSAGE '" << message.toStdString() << endl;
			}
			if(mSocket->bytesAvailable() > 0) //There may be residual bytes from a new message.
			{
				dataRecieved();
			}
		}
	}
}
