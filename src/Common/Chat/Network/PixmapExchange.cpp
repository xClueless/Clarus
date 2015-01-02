//#include "PixmapExchange.hpp"
//#include <QDebug>

//void PixmapExchange::sendPixmap()
//{
//	qDebug() << "[MessageEndpoint] Sending pixmap to remote." << endl;
//	QByteArray pixmapArray;
//	QBuffer pixmapBuffer(&pixmapArray);
//	pixmapBuffer.open(QIODevice::WriteOnly);
//	mEndpointManager->localPixmap().save(&pixmapBuffer, "PNG");
//	writeInternalMessageBytes(pixmapArray, MessageType::PIXMAP_EXCHANGE, MessageFormat::RAW);
//	mLocalPixmapState = PIXMAP_SENT;
//}
//void PixmapExchange::recievePixmap(ChatMessage* m)
//{
//	qDebug() << "[MessageEndpoint] Recieved pixmap from remote." << endl;

//	if(mRemotePixmap.loadFromData(m->messageData(), "PNG"))
//	{
//		mRemotePixmapState = PIXMAP_RECIEVED;
//		writeInternalMessageString(PIXMAP_RECIEVED_STRING, MessageType::PIXMAP_EXCHANGE);
//		emit remotePixmapChanged();
//	}
//	else
//	{
//		qDebug() << "[MessageEndpoint] Failed to load remote pixmap." << endl;
//		mRemotePixmapState = PIXMAP_NOT_SENT;
//	}
//}
