#ifndef CHATWINDOW_HPP
#define CHATWINDOW_HPP

#include <QWidget>
#include <QBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QStringList>
#include <QListView>
#include <QStringListModel>
#include "../ChatMessage.hpp"
#include "../Network/MessageClient.hpp"
#include "../ChatGroup.hpp"
#include "ConversationView.hpp"
#include "MessageLabel.hpp"
#include <QListWidget>

class ChatWindow : public QWidget
{
private:
	Q_OBJECT

	ChatGroup* mGroup;
	QStringList mMessages;
	QStringListModel mMessageModel;

	QVBoxLayout* mLayout;
	QLineEdit* mInputBox;
	QListView mConversationView;
	QListWidget mMessageListWidget;
//	ConversationView mConversationView;
public:
	explicit ChatWindow(ChatGroup* group, QWidget *parent = 0);
	QStringList endpointStringList();
	ChatGroup* group();
signals:
	void sendMessage(ChatMessage* m);
public slots:
	void messageReadyToSend();
	void displayMessage(ChatMessage* m);
	void addMessageBox(QString messageDisplayString);
};

#endif // CHATWINDOW_HPP
