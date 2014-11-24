#ifndef CLARUS_NETUTIL_HPP
#define CLARUS_NETUTIL_HPP

#include <QByteArray>
#include <QTcpSocket>

QByteArray readBytesFromSocket(QTcpSocket* socket, qint64 maxSize);

#endif // CLARUS_NETUTIL_HPP
