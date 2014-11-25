#ifndef CLARUS_NETUTIL_HPP
#define CLARUS_NETUTIL_HPP

#include <QByteArray>
#include <QTcpSocket>

QByteArray readBytesFromSocket(QAbstractSocket* socket, qint64 maxSize);

#endif // CLARUS_NETUTIL_HPP
