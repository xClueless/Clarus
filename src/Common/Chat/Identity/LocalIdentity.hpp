#ifndef LOCALIDENTITY_HPP
#define LOCALIDENTITY_HPP

#include "Identity.hpp"
#include "../Resource/LocalStringResource.hpp"
#include "../Resource/LocalPixmap.hpp"
#include "../ResourceManager.hpp"

class LocalIdentity : public Identity
{
private:
	LocalStringResource* mName;
	LocalPixmapResource* mPicture;

public:
	LocalIdentity(QString name="DefaultName",
				  QPixmap picture=QPixmap(DEFAULT_PICTURE_URL),
				  QObject* parent=0);
	~LocalIdentity();

public slots:
	LocalStringResource* name();
	LocalPixmapResource* picture();
};

#endif // LOCALIDENTITY_HPP
