//#ifndef PIXMAPEXCHANGE_HPP
//#define PIXMAPEXCHANGE_HPP

//#include <QObject>
//#include "../ChatMessage.hpp"
//#include "../LocalResource.hpp"
//#include "../RemoteResource.hpp"

//class PixmapExchange : public QObject
//{
//private:
//	Q_OBJECT
//	LocalResource mLocalPixmap;
//	RemoteResource mRemotePixmap;
//	const QString PIXMAP_REQUEST_STRING = "SEND_PIXMAP";
//	void sendUTF8Message(QString message);
//public:
//	explicit PixmapExchange(QObject *parent = 0);

//	QString stateString();
//signals:
//	void sendMessage(ChatMessage* m);
//	void protocolError(QString error);

//protected slots:
//	void notifyRemoteAboutPixmapUpdate();
//	void recievePixmap(ChatMessage* m);

//public slots:
//	void handleMessage(ChatMessage* pixmapMessage);
//	void sendPixmap();
//};

//#endif // PIXMAPEXCHANGE_HPP
