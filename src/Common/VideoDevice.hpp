#ifndef VIDEOFEED_HPP
#define VIDEOFEED_HPP

#include <string>
#include <opencv2/opencv.hpp>

class VideoDevice
{
private:
	std::string mWindowName;
	cv::VideoCapture mVideoCapture;
public:
	VideoDevice(int deviceNumber=0, std::string windowName="VideoWindow");
	void createWindow(std::string windowName);
	void paint();

	cv::Mat readFrame();
};

#endif // VIDEOFEED_HPP
