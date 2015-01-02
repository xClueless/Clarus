#include "main.hpp"

#include <iostream>

#include <thread>

#include <QApplication>
#include "Chat/Network/EndpointManager.hpp"
#include "Chat/UI/ChatWindow.hpp"
#include "Chat/UI/MainWindow.hpp"
#include "Chat/UI/LoginWidget.hpp"
#include "Chat/ResourceManager.hpp"
#include "Chat/Resource/LocalResource.hpp"
#include "Chat/Identity/LocalIdentity.hpp"

using namespace std;

#define VIDEO_SERVER_PORT 9007
#define AUDIO_SERVER_PORT 9008
#define CHAT_SERVER_PORT 9009

void qtThread(int argc, char** argv, options opts)
{
	QApplication a(argc, argv);

	QString clientName(opts.clientName.data());
	LocalIdentity identity(clientName);

	ResourceManager resourceManager;
	resourceManager.addLocalResource(identity.name());
	resourceManager.addLocalResource(identity.picture());

	EndpointManager endpointManager(CHAT_SERVER_PORT);

	LoginWidget loginWidget(&identity, &endpointManager);
	loginWidget.show();
//	MainWindow mainWindow(&endpointManager);
//	mainWindow.show();

	a.exec();
}


options processArguments(int argc, char* argv[])
{
	options opts = {"defaultName"};

	if(argc >= 2)
	{
		opts.clientName = argv[1];
	}

	return opts;
}

int main(int argc, char** argv)
{
	options opts = processArguments(argc, argv);
	qtThread(argc, argv, opts);
	return 0;
}
