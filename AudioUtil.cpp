#include "AudioUtil.hpp"


using namespace std;
using namespace std::experimental;

void printDevices()
{
	RtAudio audio;

	for(size_t i=0;i<audio.getDeviceCount();i++)
	{
		printDevice(i);
	}
}

optional<uint32_t> findDeviceID(string name, bool contains)
{
	RtAudio audio;
	optional<uint32_t> deviceID;

	RtAudio::DeviceInfo devInfo;
	for(size_t i=0;i<audio.getDeviceCount();i++)
	{
		devInfo = audio.getDeviceInfo(i);
		if(devInfo.probed)
		{
			if(contains)
			{
				if(devInfo.name.find(name) != string::npos)
				{
					deviceID = i;
					break;
				}
			}
			else if(devInfo.name == name)
			{
				deviceID = i;
				break;
			}
		}
	}
	return deviceID;
}

optional<RtAudio::DeviceInfo> findDeviceInfo(uint32_t deviceID)
{
	RtAudio audio;
	optional<RtAudio::DeviceInfo> device;

	RtAudio::DeviceInfo devInfo;
	for(size_t i=0;i<audio.getDeviceCount();i++)
	{
		devInfo = audio.getDeviceInfo(i);
		if(i==deviceID)
		{
			device = devInfo;
		}
	}
	return device;
}

optional<RtAudio::DeviceInfo> findDeviceInfo(string name, bool contains)
{
	RtAudio audio;
	optional<RtAudio::DeviceInfo> device;

	RtAudio::DeviceInfo devInfo;
	for(size_t i=0;i<audio.getDeviceCount();i++)
	{
		devInfo = audio.getDeviceInfo(i);
		if(devInfo.probed)
		{
			if(contains)
			{
				if(devInfo.name.find(name) != string::npos)
				{
					device = devInfo;
					break;
				}
			}
			else if(devInfo.name == name)
			{
				device = devInfo;
				break;
			}
		}
	}
	return device;
}

void printDevice(uint32_t deviceID)
{
	optional<RtAudio::DeviceInfo> getInfo = findDeviceInfo(deviceID);

	if(getInfo)
	{
		RtAudio::DeviceInfo devInfo = *getInfo;
		cout << "Device: " << devInfo.name << " OCs: " << devInfo.outputChannels << " ICs:" << devInfo.inputChannels << endl;
	}
	else
	{
		cout << "Device not found" << endl;
	}

}

