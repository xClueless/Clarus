#ifndef ENDPOINTBUTTON_HPP
#define ENDPOINTBUTTON_HPP

#include <QPushButton>
#include <QLabel>
#include <QBoxLayout>
#include <QList>
#include <QString>
#include "../ChatGroup.hpp"

class EndpointButton : public QPushButton
{
private:
	Q_OBJECT
	QLabel mLabel;
	QVBoxLayout mLayout;
protected:
	ChatGroup* mGroup;
public:
	explicit EndpointButton(ChatGroup* group, QWidget *parent = 0);
	ChatGroup* group();
signals:

protected slots:
	void updateLabel();
};

#endif // ENDPOINTBUTTON_HPP
