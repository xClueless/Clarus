#include "SelectEndpointsDialog.hpp"


SelectEndpointsDialog::SelectEndpointsDialog(EndpointManager* endpointManager, QWidget* parent)
	: QDialog(parent), mEndpointManager(endpointManager)
{
	setWindowModality(Qt::WindowModal);


}
