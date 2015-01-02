#ifndef RESOURCEMESSAGE_HPP
#define RESOURCEMESSAGE_HPP

#include "../ChatMessage.hpp"

class ResourceMessage
{
public:
	enum ResourceOperation
	{
		SENT_UPDATED_DATA,
		SENT_UPDATED_STATE
	};

private:
	ChatMessage* mMessage;
public:
	ResourceMessage(ChatMessage* message, QString resourceName, int state);
	ResourceMessage(ChatMessage* message, QString resourceName, const QByteArray& data);
	ResourceMessage(ChatMessage* message);

	QString resourceName() const;
	ResourceOperation resourceOperation() const;
	QByteArray resourceData();
	int resourceState() const;

	~ResourceMessage();
};

#endif // RESOURCEMESSAGE_HPP
