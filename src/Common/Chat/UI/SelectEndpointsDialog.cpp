#include "SelectEndpointsDialog.hpp"


SelectEndpointsDialog::SelectEndpointsDialog(ClientManager* clientManager, QWidget* parent)
	: QDialog(parent), mClientManager(clientManager)
{
	setWindowModality(Qt::WindowModal);


}
