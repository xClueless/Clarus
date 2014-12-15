#ifndef SELECTENDPOINTSDIALOG_HPP
#define SELECTENDPOINTSDIALOG_HPP

#include <QWidget>
#include <QDialog>
#include "../Network/EndpointManager.hpp"

class SelectEndpointsDialog : public QDialog
{
private:
	Q_OBJECT
	EndpointManager* mEndpointManager;
public:
	explicit SelectEndpointsDialog(EndpointManager* endpointManager, QWidget *parent = 0);

signals:

public slots:

};

#endif // SELECTENDPOINTSDIALOG_HPP
