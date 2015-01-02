#include "RemoteIdentity.hpp"

RemoteIdentity::RemoteIdentity(QObject* parent)
	: Identity(parent)
{
	mName = new RemoteStringResource("Identity/Name", this);
	mPicture = new RemotePixmapResource("Identity/Picture", this);
}

RemoteIdentity::~RemoteIdentity()
{

}

RemoteStringResource* RemoteIdentity::name()
{
	return mName;
}

RemotePixmapResource* RemoteIdentity::picture()
{
	return mPicture;
}
