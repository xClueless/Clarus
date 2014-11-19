#ifndef REMOTEVIDEOFEED_HPP
#define REMOTEVIDEOFEED_HPP

#include <clews/sock/Socket.hpp>
#include <opencv2/opencv.hpp>

class RemoteVideoFeed
{
private:
	SocketConnectionBase& mSocketConnection;
	int mFrameRows;
	int mFrameColumns;
	int mFrameType;
public:
	RemoteVideoFeed(SocketConnectionBase& sc);

	cv::Mat readFrame();
};

#endif // REMOTEVIDEOFEED_HPP
