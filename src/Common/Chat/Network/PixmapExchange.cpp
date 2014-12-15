#include "PixmapExchange.hpp"
#include <QDebug>

PixmapExchange::PixmapExchange(QObject* parent) : QObject(parent)
{

}

void PixmapExchange::sendUTF8Message(QString message)
{
	ChatMessage m(MessageFlags(MessageType::PIXMAP_EXCHANGE, MessageFormat::RAW), message);
	sendMessage(&m);
}

QString PixmapExchange::stateString()
{
	QString pixmapStateString = "Local: " + mLocalPixmap.stateString();
	pixmapStateString += " Remote: " + mRemotePixmap.stateString();
	return pixmapStateString;
}

void PixmapExchange::requestPixmap()
{
	qDebug() << "[MessageEndpoint] Requesting remote pixmap." << endl;
	sendUTF8Message(ChatPixmap::);
	mRemotePixmap.setState(PIXMAP_REQUESTED);
}

void PixmapExchange::sendPixmap()
{
	qDebug() << "[MessageEndpoint] Sending pixmap to remote." << endl;
	QByteArray pixmapArray;
	QBuffer pixmapBuffer(&pixmapArray);
	pixmapBuffer.open(QIODevice::WriteOnly);
	mEndpointManager->localPixmap().save(&pixmapBuffer, "PNG");
	writeInternalMessageBytes(pixmapArray, MessageType::PIXMAP_EXCHANGE, MessageFormat::RAW);
	mLocalPixmapState = PIXMAP_SENT;
}

