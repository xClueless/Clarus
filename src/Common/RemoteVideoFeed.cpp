#include "RemoteVideoFeed.hpp"

using namespace std;
using namespace cv;

RemoteVideoFeed::RemoteVideoFeed(SocketConnectionBase& sc) : mSocketConnection(sc)
{
	cout << "Requesting rows" << endl;
	mSocketConnection.write("SEND-FRAME-ROWS");
	mFrameRows = mSocketConnection.readInt32();

	cout << "Requesting columns" << endl;
	mSocketConnection.write("SEND-FRAME-COLUMNS");
	mFrameColumns = mSocketConnection.readInt32();

	cout << "Requesting type" << endl;
	mSocketConnection.write("SEND-FRAME-TYPE");
	mFrameType = mSocketConnection.readInt32();
}

Mat RemoteVideoFeed::readFrame()
{
	mSocketConnection.write("SEND-FRAME");
	vector<char> frameVector = mSocketConnection.read();
	Mat m(mFrameRows, mFrameColumns, mFrameType);
	memcpy((char*) m.data, &frameVector.at(0), frameVector.size());
	return m;
}
