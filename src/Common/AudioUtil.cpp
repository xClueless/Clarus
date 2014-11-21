#include "AudioUtil.hpp"


using namespace std;

void printDevices()
{
	RtAudio audio;

	for(size_t i=0;i<audio.getDeviceCount();i++)
	{
		printDevice(i);
	}
}

pair<uint32_t, bool> findDeviceID(string name, bool contains)
{
	RtAudio audio;
	pair<uint32_t, bool> deviceID;
	deviceID.second = false;

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
					deviceID.first = i;
					deviceID.second = true;
					break;
				}
			}
			else if(devInfo.name == name)
			{
				deviceID.first = i;
				deviceID.second = true;
				break;
			}
		}
	}
	return deviceID;
}

//Should really be an optional
pair<RtAudio::DeviceInfo, bool> findDeviceInfo(uint32_t deviceID)
{
	RtAudio audio;
	pair<RtAudio::DeviceInfo, bool> device;
	device.second = false;

	RtAudio::DeviceInfo devInfo;
	for(size_t i=0;i<audio.getDeviceCount();i++)
	{
		devInfo = audio.getDeviceInfo(i);
		if(i==deviceID)
		{
			device.first = devInfo;
			device.second = true;
		}
	}
	return device;
}

pair<RtAudio::DeviceInfo, bool> findDeviceInfo(string name, bool contains)
{
	RtAudio audio;
	pair<RtAudio::DeviceInfo, bool> device;
	device.second = false;

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
					device.first = devInfo;
					device.second = true;
					break;
				}
			}
			else if(devInfo.name == name)
			{
				device.first = devInfo;
				device.second = true;
				break;
			}
		}
	}
	return device;
}

void printDevice(uint32_t deviceID)
{
	pair<RtAudio::DeviceInfo, bool> getInfo = findDeviceInfo(deviceID);

	if(getInfo.second)
	{
		RtAudio::DeviceInfo devInfo = getInfo.first;
		cout << "Device: " << devInfo.name << " OCs: " << devInfo.outputChannels << " ICs:" << devInfo.inputChannels << endl;
	}
	else
	{
		cout << "Device not found" << endl;
	}

}

