#include "VideoServer.hpp"
#include <clews/sock/SocketConnection.hpp>

using namespace std;
using namespace cv;

VideoServer::VideoServer(VideoDevice& vd, int portNumber) : mVideoDevice(vd), mSocket(portNumber)
{
}

void VideoServer::start()
{
	mSendData = true;

	mSocket.create();
	mSocket.listen();

	cout << "Server up." << endl;
	SocketConnection* client = mSocket.accept();
	client->setMessageWrapping(true);

	while(mSendData)
	{

		vector<char> clientData = client->read();
		string clientRequest(clientData.begin(), clientData.end());
		cout << "Got client request: " << clientRequest << endl;

		if(clientRequest == "SEND-FRAME-ROWS")
		{
			cout << "Sending rows" << endl;
			client->writeInt32(mVideoDevice.readFrame().rows);
		}
		else if(clientRequest == "SEND-FRAME-COLUMNS")
		{
			cout << "Sending columns" << endl;
			client->writeInt32(mVideoDevice.readFrame().cols);
		}
		else if(clientRequest == "SEND-FRAME-TYPE")
		{
			cout << "Sending type" << endl;
			client->writeInt32(mVideoDevice.readFrame().type());
		}
		else if(clientRequest == "SEND-FRAME")
		{
			cout << "Sending frame" << endl;
			Mat frame = mVideoDevice.readFrame();
			int frameSize = frame.dataend - frame.datastart;
			client->write((const char*) frame.data, frameSize);
		}
		else
		{
			client->write("ERROR-UKNOWN-REQUEST");
		}
	}
}

void VideoServer::stop()
{
	mSendData = false;
}
