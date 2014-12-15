#ifndef PIXMAPEXCHANGE_HPP
#define PIXMAPEXCHANGE_HPP

#include <QObject>
#include "../ChatMessage.hpp"
#include "../ChatPixmap.hpp"

class PixmapExchange : public QObject
{
private:
	Q_OBJECT
	ChatPixmap mLocalPixmap;
	ChatPixmap mRemotePixmap;

	void sendUTF8Message(QString message);
public:
	explicit PixmapExchange(QObject *parent = 0);

	QString stateString();
signals:
	void sendMessage(ChatMessage* m);
	void protocolError(QString error);

public slots:
};

#endif // PIXMAPEXCHANGE_HPP
