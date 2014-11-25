#include "main.hpp"

#include <iostream>

#include <thread>

//#include "Video/VideoServer.hpp"
//#include "Video/RemoteVideoFeed.hpp"

//#include "Audio/AudioUtil.hpp"
//#include "Audio/AudioBouncer.hpp"

//#include "TimeUtil.h"

#include <QApplication>
#include "Chat/Network/ClientManager.hpp"
#include "Chat/UI/ChatWindow.hpp"
#include "Chat/UI/ContactWindow.hpp"

using namespace std;
using namespace cv;

#define VIDEO_SERVER_PORT 9007
#define AUDIO_SERVER_PORT 9008
#define CHAT_SERVER_PORT 9009

//void vdSenderThread(VideoDevice* vd)
//{
//	try
//	{
//		VideoServer vdServer(*vd, VIDEO_SERVER_PORT);
//		vdServer.start();
//	}
//	catch(const runtime_error& re)
//	{
//		cerr << re.what();
//		exit(-1);
//	}
//}
//void vdViewerThread(std::string remoteHost)
//{
//	try
//	{
//		SocketClient serverConnection;
//		serverConnection.open(remoteHost, VIDEO_SERVER_PORT);
//		serverConnection.setMessageWrapping(true);

//		RemoteVideoFeed rfv(serverConnection);

//		namedWindow("Remote Feed");

//		char pressedKey = 'z';
//		while(pressedKey != 'a')
//		{
//			imshow("Remote Feed", rfv.readFrame());
//			pressedKey = waitKey(30);
//		}
//	}
//	catch(const runtime_error& re)
//	{
//		cerr << re.what();
//		exit(-1);
//	}
//}
//void vdPainterThread(VideoDevice* vd)
//{
//	char pressedKey = 'z';
//	while(pressedKey != 'q')
//	{
//		vd->paint();
//		pressedKey = waitKey(30);
//	}
//}

//void rtaListenerThread(std::string remoteHost)
//{
//	try
//	{
//		SocketClient serverConnection;
//		serverConnection.open(remoteHost, AUDIO_SERVER_PORT);
//		serverConnection.setMessageWrapping(true);

//		AudioBouncer bouncer(&serverConnection);
//		bouncer.recieve();

//		while(true)
//		{
//		}
//	}
//	catch(const runtime_error& re)
//	{
//		cerr << re.what();
//		exit(-1);
//	}
//	catch(const RtAudioError& rae)
//	{
//		rae.printMessage();
//		exit(-1);
//	}
//}

//void rtaSenderThread()
//{
//	try
//	{
//		Socket server(AUDIO_SERVER_PORT);
//		server.create();
//		server.listen();
//		SocketConnection* client = server.accept();
//		client->setMessageWrapping(true);

//		AudioBouncer bouncer(client);
//		bouncer.send();
//		while(true)
//		{
//		}
//	}
//	catch(const runtime_error& re)
//	{
//		cerr << re.what();
//		exit(-1);
//	}
//}
void qtThread(int argc, char** argv, options opts)
{
	QApplication a(argc, argv);

	ClientManager clientManager(opts.clientName.data(), CHAT_SERVER_PORT);
	try
	{
		clientManager.start();
	}
	catch(const runtime_error& re)
	{
		cerr << re.what() << endl;
	}

	ContactWindow contactWindow(&clientManager);
	contactWindow.show();

	QObject::connect(&clientManager, SIGNAL(endpointIdentified(MessageEndpoint*)), &contactWindow, SLOT(endpointIdentified(MessageEndpoint*)));
	clientManager.connectToServer("127.0.0.1");

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
//	init_net();

	qtThread(argc, argv, opts);


//	thread audioSenderThread(rtaSenderThread);
//	sleepSeconds(1);
//	thread audioListenerThread(rtaListenerThread, opts.remoteHost);

//	VideoDevice vd;
//	thread videoSenderThread(vdSenderThread, &vd);
//	sleepSeconds(1);
//	thread videoViewerThread(vdViewerThread, opts.remoteHost);
//	thread videoPainterThread(vdPainterThread, &vd);

//	videoSenderThread.join();
//	videoViewerThread.join();
//	videoPainterThread.join();

//	audioSenderThread.join();
//	audioListenerThread.join();

//	cleanup_net();
	return 0;
}
