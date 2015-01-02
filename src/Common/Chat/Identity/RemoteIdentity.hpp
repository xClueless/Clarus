#ifndef REMOTEIDENTITY_HPP
#define REMOTEIDENTITY_HPP

#include "Identity.hpp"
#include "../Resource/RemoteStringResource.hpp"
#include "../Resource/RemotePixmap.hpp"


class RemoteIdentity : public Identity
{
private:
	RemoteStringResource* mName;
	RemotePixmapResource* mPicture;
public:
	RemoteIdentity(QObject* parent=0);

	~RemoteIdentity();

	RemoteStringResource* name();
	RemotePixmapResource* picture();
};

#endif // REMOTEIDENTITY_HPP
