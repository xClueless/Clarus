#include "LocalIdentity.hpp"

LocalIdentity::LocalIdentity(QString name, QPixmap picture, QObject* parent)
	: Identity(parent)
{
	mName = new LocalStringResource("Identity/Name", name, this);

	mPicture = new LocalPixmapResource("Identity/Picture", this);
	mPicture->setPixmap(picture);
}

LocalIdentity::~LocalIdentity()
{
}

LocalStringResource* LocalIdentity::name()
{
	return mName;
}

LocalPixmapResource* LocalIdentity::picture()
{
	return mPicture;
}
