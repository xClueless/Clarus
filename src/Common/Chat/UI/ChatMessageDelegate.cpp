//#include "ChatMessageDelegate.hpp"

//ChatMessageDelegate::ChatMessageDelegate(QWidget* parent)
//	: QStyledItemDelegate(parent)
//{
//}


//void ChatMessageDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
//{
//	QString messageDisplayString = index.data().toString();
//	QString sender = messageDisplayString.section(':', 0, 0);
//	QString messageString = messageDisplayString.section(':', 1);

//	if(mMessageLabel != NULL)
//	{
//		delete mMessageLabel->message();
//		delete mMessageLabel;
//	}
//	ChatMessage* message = new ChatMessage(messageString, sender);
//	mMessageLabel = new MessageLabel(message);
//	mMessageLabel->
//	layout()->addWidget(mMessageLabel);
//}
