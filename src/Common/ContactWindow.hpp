#ifndef CONTACTWINDOW_HPP
#define CONTACTWINDOW_HPP

#include <QWidget>
#include <QString>
#include <QLineEdit>
#include <QGridLayout>
#include <QPushButton>
#include "ChatWindow.hpp"
#include "MessageMarshaller.hpp"

class ContactWindow : public QWidget
{
private:
	QGridLayout* mLayout;
	QList<ChatWindow*> mChatWindows;
	QList<QPushButton*> mContactButtons;
	MessageMarshaller* mMarshaller;
	Q_OBJECT
public:
	explicit ContactWindow(MessageMarshaller* marshaller, QWidget *parent = 0);

signals:
	void connectToClient(QString remoteHostname);
public slots:
	void contactButtonClicked();
};

#endif // CONTACTWINDOW_HPP
