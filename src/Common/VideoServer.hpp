#ifndef VIDEOSERVER_HPP
#define VIDEOSERVER_HPP

#include "VideoDevice.hpp"
#include <opencv2/opencv.hpp>
#include <clews/sock/Socket.hpp>

class VideoServer
{
private:
	VideoDevice& mVideoDevice;
	bool mSendData = false;
	Socket mSocket;
public:
	VideoServer(VideoDevice& vd, int portNumber);
	void start();
	void stop();
};

#endif // VIDEOSERVER_HPP
