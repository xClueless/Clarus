#ifndef IDENTITY_HPP
#define IDENTITY_HPP

#include <QObject>
#include <QString>
#include <QMap>

enum class IdentState
{
	NOT_IDENTIFIED,
	REQUESTED,
	SENT,
	IDENTIFIED,
};

class Identity : public QObject
{
private:
	Q_OBJECT
	IdentState mState;
	QString mName;

	static QMap<IdentState, QString> create_ident_string_map();
	static QMap<IdentState, QString> mIdentStringMap;
public:
	static QString REQUEST_STRING;
	static QString IDENTIFIED_STRING;
	static QString UPDATE_AVAILABLE_STRING;
	static QString SENT_EMPTY_NAME_STRING;
	static QString UNSPECIFIED_IDENT_ERROR_STRING;
	static QString REQUIRES_IDENTIFICATON_STRING;

	Identity(QObject* parent=0);
	IdentState state() const;
	QString stateString() const;
	static QString stateString(IdentState state);
	QString name() const;
public slots:
	void setName(const QString& name);
	void setState(IdentState state);
};

#endif // IDENTITY_HPP
