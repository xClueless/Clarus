#ifndef MAIN_HPP
#define MAIN_HPP

#include <string>
#include "Video/VideoDevice.hpp"

struct options
{
	std::string clientName;
};

options processArguments(int argc, char* argv[]);
void rtaListenerThread(std::string remoteHost);
void rtaSenderThread();
void vdPainterThread(VideoDevice* vd);
void vdSenderThread(VideoDevice* vd);
void vdViewerThread(std::string remoteHost);



#endif // MAIN_HPP
