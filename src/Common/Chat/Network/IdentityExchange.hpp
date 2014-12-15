#ifndef IDENTITYEXCHANGE_HPP
#define IDENTITYEXCHANGE_HPP

#include "../Identity.hpp"
#include "../ChatMessage.hpp"
#include "../ConnectionError.hpp"

#include <QObject>

class IdentityExchange : public QObject
{
private:
	Q_OBJECT
	Identity mLocalIdentity;
	Identity mRemoteIdentity;

	void setRemoteName(QString name);
	void sendUTF8Message(QString message);
public:
	IdentityExchange(QObject* parent=0);
	bool identificationIsComplete();
	QString identityExchangeStateString();

signals:
	void protocolError(QString error);
	void remoteIdentified();
	void remoteIdentifiedUs();
	void identificationSuccessful();
	void identificationFailed(ConnectionError connectionError);
	void unidentifiedClientSentMessage();
	void remoteNameChanged();
	void sendMessage(ChatMessage* m);

public slots:
	void requestIdentity();
	void process(ChatMessage* request);
	void sendIdentityRequiredError();
	void notifyRemoteAboutIdentityUpdate();
	void sendIdentity();
	void identityRecieved(QString remoteName);
};

#endif // IDENTITYEXCHANGE_HPP
