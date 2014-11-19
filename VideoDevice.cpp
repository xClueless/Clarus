#include "VideoDevice.hpp"
#include <stdexcept>

using namespace std;
using namespace cv;

VideoDevice::VideoDevice(int deviceNumber, string windowName) : mVideoCapture(deviceNumber), mWindowName(windowName)
{
	if(!mVideoCapture.isOpened())
	{
		throw runtime_error("Failed to open capture device");
	}

	createWindow(mWindowName);
}

void VideoDevice::createWindow(string windowName)
{
	mWindowName = windowName;
	namedWindow(mWindowName, CV_WINDOW_AUTOSIZE);
}

void VideoDevice::paint()
{
	Mat frame;
	mVideoCapture >> frame;

	imshow(mWindowName, frame);
	waitKey(5); //Needed to actually paint.
}

Mat VideoDevice::readFrame()
{
	Mat frame;
	mVideoCapture >> frame;
	return frame;
}

