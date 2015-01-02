#ifndef IDENTITY_HPP
#define IDENTITY_HPP

#include <QObject>
#include "../ResourceManager.hpp"
#include "../Resource/ChatResource.hpp"


class Identity : public QObject
{
public:
	static QString DEFAULT_PICTURE_URL;

	Identity(QObject* parent);
	~Identity();

	virtual ChatResource* name()=0;
	virtual ChatResource* picture()=0;
};

#endif // IDENTITY_HPP
