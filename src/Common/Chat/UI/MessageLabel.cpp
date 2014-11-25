#include "MessageLabel.hpp"
#include <QVariant>

void MessageLabel::setLabel(ChatMessage* m)
{
	QString senderTags = "<b>%1: </b>";
	if(m->sender() == "Me")
	{
		setAlignment(Qt::AlignLeft | Qt::AlignTop);
		setProperty("SelfMessage", QVariant(true));
		senderTags = "<font color=\"red\">" + senderTags + "</font>";
	}
	else
	{
		setAlignment(Qt::AlignRight | Qt::AlignTop);
	}

	setText(senderTags.arg(m->sender()) + m->message());
}

MessageLabel::MessageLabel(ChatMessage* m, QWidget* parent)
	: QLabel(parent), mMessage(m)
{
	setTextFormat(Qt::RichText);
	setLabel(mMessage);
}

ChatMessage* MessageLabel::message()
{
	return mMessage;
}
