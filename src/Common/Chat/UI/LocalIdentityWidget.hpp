#ifndef LOCALIDENTITYWIDGET_HPP
#define LOCALIDENTITYWIDGET_HPP

#include <QWidget>
#include <QBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include "../Network/EndpointManager.hpp"

class LocalIdentityWidget : public QWidget
{
private:
	Q_OBJECT
	EndpointManager* mEndpointManager;
	QHBoxLayout* mLayout;
	QPushButton* mLocalPixmapButton;
	QLabel* mLocalNameLabel;
	QLineEdit* mLocalNameBox;
public:
	explicit LocalIdentityWidget(EndpointManager* endpointManager, QWidget *parent = 0);

signals:

public slots:
	void sendNewLocalName();
	void retrieveNewLocalName();
	void selectNewPixmap();
	void updatePixmap();
};

#endif // LOCALIDENTITYWIDGET_HPP
