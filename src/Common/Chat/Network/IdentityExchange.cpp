#include "IdentityExchange.hpp"
#include <QDebug>

void IdentityExchange::IdentityExchange::setRemoteName(QString name)
{
	mRemoteIdentity.setName(name);
	emit remoteNameChanged();
}

void IdentityExchange::sendUTF8Message(QString message)
{
	ChatMessage m(MessageFlags(MessageType::IDENTIFICATION, MessageFormat::UTF8), message);
	sendMessage(&m);
}

IdentityExchange::IdentityExchange(QObject* parent) : QObject(parent)
{
}

bool IdentityExchange::identificationIsComplete()
{
	return mLocalIdentity.state() == IdentState::IDENTIFIED && mRemoteIdentity.state() == IdentState::IDENTIFIED;
}

void IdentityExchange::requestIdentity()
{
	qDebug() << "[IdentityExchange] Requesting identification from remote." << endl;
	sendUTF8Message(Identity::REQUEST_STRING);
	mRemoteIdentity.setState(IdentState::REQUESTED);
}

void IdentityExchange::process(ChatMessage* request)
{
	QString requestString = request->messageDataAsUTF8String();

	if(mRemoteIdentity.state() == IdentState::IDENTIFIED
			&& requestString == Identity::UPDATE_AVAILABLE_STRING)
	{
		requestIdentity();
	}
	if(mRemoteIdentity.state() == IdentState::REQUESTED)
	{
		identityRecieved(requestString);
	}
	else if(mLocalIdentity.state() == IdentState::NOT_IDENTIFIED
			&& requestString == Identity::REQUEST_STRING)
	{
		mLocalIdentity.setState(IdentState::REQUESTED);
		sendIdentity();
	}
	else if(mLocalIdentity.state() == IdentState::SENT)
	{
		if(requestString == Identity::IDENTIFIED_STRING)
		{
			qDebug() << "[IdentityExchange] Remote successfully identified us." << endl;
			mLocalIdentity.setState(IdentState::IDENTIFIED);
			emit remoteIdentifiedUs();
		}
		else
		{
			qDebug() << "[IdentityExchange] An unhandled protocol error occured: " << request->messageDataAsUTF8String() << endl;
			mLocalIdentity.setState(IdentState::NOT_IDENTIFIED);
			emit identificationFailed(ConnectionError(IDENT_UNSPECIFIED_ERROR));
		}
	}
	else
	{
		qDebug() << "[IdentityExchange] Identity message: " << requestString
			 << " not understood in current state: " << identityExchangeStateString() << endl;
		protocolError("UNKNOWN_COMMAND");
	}

	if(mLocalIdentity.state() == IdentState::IDENTIFIED
			&& mRemoteIdentity.state() == IdentState::IDENTIFIED)
	{
		qDebug() << "[IdentityExchange] Identification complete." << endl;
		emit identificationSuccessful();
	}

}

QString IdentityExchange::identityExchangeStateString()
{
	return "Local: " + mLocalIdentity.stateString() + " Remote: " + mRemoteIdentity.stateString();
}

void IdentityExchange::notifyRemoteAboutIdentityUpdate()
{
	sendUTF8Message(Identity::UPDATE_AVAILABLE_STRING);
}

void IdentityExchange::sendIdentityRequiredError()
{
	sendUTF8Message(Identity::REQUIRES_IDENTIFICATON_STRING);
	emit unidentifiedClientSentMessage();
}

void IdentityExchange::sendIdentity()
{
	qDebug() << "[IdentityExchange] Sending identity to remote." << endl;
	sendUTF8Message(mLocalIdentity.name());
	mLocalIdentity.setState(IdentState::SENT);
}
void IdentityExchange::identityRecieved(QString remoteName)
{
	if(remoteName.isEmpty())
	{
		sendUTF8Message(Identity::SENT_EMPTY_NAME_STRING);

		emit protocolError("[IdentityExchange] Remote host '%r' violated IDENTIFY protocol. They sent back an empty name.");
		mRemoteIdentity.setState(IdentState::NOT_IDENTIFIED);
		emit identificationFailed(ConnectionError(IDENT_SENT_EMPTY_NAME));
	}
	else
	{
		sendUTF8Message(Identity::IDENTIFIED_STRING);
		setRemoteName(remoteName);
		mRemoteIdentity.setState(IdentState::IDENTIFIED);
		qDebug() << "[IdentityExchange] Remote successfully identified with name: " << mRemoteIdentity.name() << endl;
		emit remoteIdentified();
	}
}

