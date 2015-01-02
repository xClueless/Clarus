#ifndef MESSAGEHANDLER_HPP
#define MESSAGEHANDLER_HPP

#include <QObject>
#include "../ChatMessage.hpp"

class MessageHandler : public QObject
{
private:
	Q_OBJECT
	QString mHandlerName;
	ChatMessage::MessageType mDefaultType = ChatMessage::MessageType::ERROR;
public:
	explicit MessageHandler(QString handlerName, QObject* parent=0);

	~MessageHandler();
	QString messageHandlerName();
signals:
	void writeChatMessage(ChatMessage* m);
public slots:
	virtual void handleMessage(ChatMessage* m)=0;
};

#endif // MESSAGEHANDLER_HPP
