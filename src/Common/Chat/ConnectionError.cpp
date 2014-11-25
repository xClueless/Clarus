#include "ConnectionError.hpp"

ConnectionError::ConnectionError(ConnectionErrorEnum ce)
	: mError(ce)
{
}
ConnectionError::ConnectionError(QAbstractSocket* socket, QAbstractSocket::SocketError error) : mError(SOCKET_ERROR), mSocketError(error)
{
	mSocketErrorString = socket->errorString();
}
ConnectionErrorEnum ConnectionError::error()
{
	return mError;
}
QAbstractSocket::SocketError ConnectionError::socketError()
{
	return mSocketError;
}
QString ConnectionError::errorString()
{
	switch(mError)
	{
		case NET_CONNECT_FAILED: return NET_CONNECT_FAILED_STRING;
		case IDENT_SENT_EMPTY_NAME: return IDENT_SENT_EMPTY_NAME_STRING;
		case IDENT_UNSPECIFIED_ERROR: return IDENT_UNSPECIFIED_ERROR_STRING;
		case SOCKET_ERROR: return mSocketErrorString;
		default: return UNSPECIFIED_ERROR_STRING;
	}
}

