#ifndef SELECTENDPOINTSDIALOG_HPP
#define SELECTENDPOINTSDIALOG_HPP

#include <QWidget>
#include <QDialog>
#include "../Network/ClientManager.hpp"

class SelectEndpointsDialog : public QDialog
{
private:
	Q_OBJECT
	ClientManager* mClientManager;
public:
	explicit SelectEndpointsDialog(ClientManager* clientManager, QWidget *parent = 0);

signals:

public slots:

};

#endif // SELECTENDPOINTSDIALOG_HPP
