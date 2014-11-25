#ifndef CONNECTIONERROR_HPP
#define CONNECTIONERROR_HPP

#include <QString>
#include <QAbstractSocket>

enum ConnectionErrorEnum
{
	NET_CONNECT_FAILED,
	IDENT_SENT_EMPTY_NAME,
	IDENT_UNSPECIFIED_ERROR,
	UNSPECIFIED_ERROR,
	SOCKET_ERROR
};

class ConnectionError
{
private:
	ConnectionErrorEnum mError = SOCKET_ERROR;
	QAbstractSocket::SocketError mSocketError;
	QString mSocketErrorString;
public:
	const QString NET_CONNECT_FAILED_STRING = "NET_CONNECT_FAILED";
	const QString IDENT_SENT_EMPTY_NAME_STRING = "IDENT_SENT_EMPTY_NAME";
	const QString IDENT_UNSPECIFIED_ERROR_STRING = "IDENT_UNSPECIFIED_ERROR";
	const QString UNSPECIFIED_ERROR_STRING = "UNSPECIFIED_ERROR";

	ConnectionError(ConnectionErrorEnum ce);
	ConnectionError(QAbstractSocket* socket, QAbstractSocket::SocketError error);

	ConnectionErrorEnum error();
	QAbstractSocket::SocketError socketError();
	QString errorString();
};

#endif // CONNECTIONERROR_HPP
