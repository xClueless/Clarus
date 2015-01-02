#ifndef LOCALIDENTITYWIDGET_HPP
#define LOCALIDENTITYWIDGET_HPP

#include <QWidget>
#include <QBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include "../Network/EndpointManager.hpp"
#include "../Identity/LocalIdentity.hpp"

class LocalIdentityWidget : public QWidget
{
private:
	Q_OBJECT
	LocalIdentity* mLocalIdentity;
	QHBoxLayout* mLayout;
	QPushButton* mLocalPixmapButton;
	QLabel* mLocalNameLabel;
	QLineEdit* mLocalNameBox;
public:
	explicit LocalIdentityWidget(LocalIdentity* localIdentity, QWidget *parent = 0);

signals:

public slots:
	void setLocalName();
	void retrieveNewLocalName();
	void selectNewPixmap();
	void updatePixmap();
};

#endif // LOCALIDENTITYWIDGET_HPP
