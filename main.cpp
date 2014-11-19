#include <iostream>
#include <clews/sock/SocketClient.hpp>

#include <thread>

#include "VideoDevice.hpp"
#include "VideoServer.hpp"
#include "RemoteVideoFeed.hpp"

using namespace std;
using namespace cv;

void serverThread()
{
	Socket s(9001);
	s.create();
	s.listen();

	cout << "[SERVER] Waiting for client" << endl;
	SocketConnection* client = s.accept();
	client->setMessageWrapping(true);

	cout << "[SERVER] Got client" << endl;

	while(true)
	{
		vector<char> clientData = client->read();
		string clientResponse(clientData.begin(), clientData.end());
		cout << "Got response: " << clientResponse << endl;

		if(clientResponse == "HELLO")
		{
			client->write("HI THERE");
		}
		else if(clientResponse == "SEND-INT32")
		{
			client->writeInt32(56);
		}
		else
		{
			client->write("UNKNOWN MESSAGE");
		}
	}
}
void clientThread()
{
	cout << "[CLIENT] Connecting to server" << endl;
	SocketClient server;
	server.setMessageWrapping(true);
	server.open("127.0.0.1", 9001);

	cout << "[CLIENT] Sending messages now" << endl;
	server.write("HELLO");
	vector<char> respData = server.read();
	cout << string(respData.begin(), respData.end()) << endl;

	server.write("TEST");
	respData = server.read();
	cout << string(respData.begin(), respData.end()) << endl;

	server.write("SEND-INT32");
	cout << "[CLIENT] Server sent back int: " << server.readInt32() << endl;

}

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
