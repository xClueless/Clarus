#include <iostream>
#include <clews/sock/SocketClient.hpp>

#include <thread>
#include <string>

#include "VideoDevice.hpp"
#include "VideoServer.hpp"
#include "RemoteVideoFeed.hpp"

#include <RtAudio.h>
#include "AudioUtil.hpp"
#include "AudioBouncer.hpp"

using namespace std;
using namespace std::experimental;
using namespace cv;

void vdServerThread(VideoDevice* vd)
{
	VideoServer vdServer(*vd, 9000);
	vdServer.start();
}
void vdClientThread(std::string remoteHost)
{
	SocketClient serverConnection;
	serverConnection.open(remoteHost, 9000);

	string connectedString = serverConnection.isOpen() ? "open" : "closed";
	cout << "Connection state: " << connectedString << endl;

	RemoteVideoFeed rfv(serverConnection);

	namedWindow("Remote Feed");

	char pressedKey = 'z';
	while(pressedKey != 'a')
	{
		imshow("Remote Feed", rfv.readFrame());
		pressedKey = waitKey(30);
	}
}
void vdPainterThread(VideoDevice* vd)
{
	char pressedKey = 'z';
	while(pressedKey != 'q')
	{
		vd->paint();
		pressedKey = waitKey(30);
	}
}

void rtaListenerThread()
{
	try
	{
		SocketClient* serverConnection = new SocketClient();
		serverConnection->open("127.0.0.1", 9001);
		serverConnection->setMessageWrapping(true);

		AudioBouncer bouncer(serverConnection);
		bouncer.recieve();

		while(true)
		{

		}
	}
	catch(const RtAudioError& rae)
	{
		rae.printMessage();
	}
}

void rtaSenderThread()
{
	Socket server(9001);
	server.create();
	server.listen();

	SocketConnection* client = server.accept();
	client->setMessageWrapping(true);

	AudioBouncer bouncer(client);
	bouncer.send();

	while(true)
	{

	}
}

int main()
{
	thread audioSenderThread(rtaSenderThread);
	sleep(1);
	thread audioListenerThread(rtaListenerThread);

	audioSenderThread.join();
	audioListenerThread.join();

//	thread listenerThread(paListenerThread);

//	//Video Stuff
//	VideoDevice* localVD = new VideoDevice(0, "Local Video");

//	thread painterThread(vdPainterThread, localVD);
//	thread serverThread(vdServerThread, localVD);

//	sleep(2);
//	thread clientThread(vdClientThread, "127.0.0.1");

//	listenerThread.join();
//	painterThread.join();
//	serverThread.join();
//	clientThread.join();

	return 0;
}
