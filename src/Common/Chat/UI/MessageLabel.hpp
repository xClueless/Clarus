#ifndef MESSAGELABEL_HPP
#define MESSAGELABEL_HPP

#include <QLabel>
#include "../ChatMessage.hpp"

class MessageLabel : public QLabel
{
private:
	Q_OBJECT
	ChatMessage* mMessage;
	void setLabel(ChatMessage* m);
public:
	MessageLabel(ChatMessage* m, QWidget* parent=0);
	ChatMessage* message();
};

#endif // MESSAGELABEL_HPP
