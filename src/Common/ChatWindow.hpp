#ifndef CHATWINDOW_HPP
#define CHATWINDOW_HPP

#include <QWidget>
#include <QBoxLayout>
#include <QTextEdit>
#include <QLineEdit>
#include <QStringList>
#include "ChatMessage.hpp"
#include "MessageClient.hpp"

class ChatWindow : public QWidget
{
private:
	Q_OBJECT

	QVBoxLayout* mLayout;
	QTextEdit* mMessageArea;
	QLineEdit* mInputBox;
	QList<MessageClient*> mConversationClients;
public:
	explicit ChatWindow(QList<MessageClient*> conversationClients=QList<MessageClient*>(), QWidget *parent = 0);
	QStringList clientStringList();
signals:
	void sendMessage(ChatMessage* m);
public slots:
	void messageReadyToSend();
	void displayMessage(ChatMessage* m);
	void addClient(MessageClient* mc);
};

#endif // CHATWINDOW_HPP
