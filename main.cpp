#include <iostream>
#include <clews/sock/SocketClient.hpp>

#include <thread>

#include "VideoDevice.hpp"
#include "VideoServer.hpp"
#include "RemoteVideoFeed.hpp"

using namespace std;
using namespace cv;

void vdServerThread()
{
	VideoDevice localVD(0, "Local Video");
	VideoServer vdServer(localVD, 9000);
	vdServer.start();
}
void vdClientThread()
{
	SocketClient serverConnection;
	serverConnection.open("127.0.0.1", 9000);

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

int main()
{
//	vdServerThread();
	vdClientThread();

	return 0;
}
