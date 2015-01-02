#ifndef CHATMESSAGE_HPP
#define CHATMESSAGE_HPP

#include <QString>
#include <QStringList>
#include <QChar>
#include <QByteArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QVariant>
#include <QMap>

class ChatMessage
{
public:
	enum MessageType
	{
		RESOURCE_EXCHANGE,
		PRIVATE,
		ERROR
	};
private:
	QString mSender = "SENDER_DEFAULT";
	const int LOCAL_API_LEVEL = 1;
	QVariantMap mSectionMap;
protected:
	MessageType mMessageType;
public:
	ChatMessage(QJsonDocument json, QString sender);
	ChatMessage(MessageType type);

	void addSection(QString sectionName, const QVariant data);
	QVariantMap& sections();
	QVariant& section(QString sectionName);
	const QJsonDocument jsonDocument();

	QString sender();
	void setSender(QString sender);

	MessageType type();
	void setType(MessageType type);
};

#endif // CHATMESSAGE_HPP
