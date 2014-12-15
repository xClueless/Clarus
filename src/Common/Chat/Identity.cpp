#include "Identity.hpp"

QMap<IdentState, QString> Identity::mIdentStringMap = create_ident_string_map();

QString Identity::REQUEST_STRING = "IDENTIFY";
QString Identity::IDENTIFIED_STRING = "IDENTIFIED";
QString Identity::UPDATE_AVAILABLE_STRING = "IDENTITY_UPDATE_AVAILABLE";
QString Identity::SENT_EMPTY_NAME_STRING = "SENT_EMPTY_NAME";
QString Identity::UNSPECIFIED_IDENT_ERROR_STRING = "UNSPECIFIED_IDENT_ERROR";
QString Identity::REQUIRES_IDENTIFICATON_STRING = "REQUIRES_IDENTIFICATON";


QMap<IdentState, QString> Identity::create_ident_string_map()
{
	QMap<IdentState, QString> identStringMap = {
		{IdentState::NOT_IDENTIFIED, "NOT_IDENTIFIED"},
		{IdentState::REQUESTED, "IDENTITY_REQUESTED"},
		{IdentState::SENT, "IDENTITY_SENT"},
		{IdentState::IDENTIFIED, "IDENTIFIED"},
	};
	return identStringMap;
}

Identity::Identity(QObject* parent) : QObject(parent), mState(IdentState::NOT_IDENTIFIED)
{
}

IdentState Identity::state() const
{
	return mState;
}

QString Identity::stateString() const
{
	return stateString(mState);
}

QString Identity::stateString(IdentState state)
{
	return mIdentStringMap[state];
}

QString Identity::name() const
{
	return mName;
}

void Identity::setName(const QString& name)
{
	mName = name;
}

void Identity::setState(IdentState state)
{
	mState = state;
}
