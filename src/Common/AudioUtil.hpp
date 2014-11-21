#ifndef AUDIOUTIL_HPP
#define AUDIOUTIL_HPP

#include <RtAudio.h>
#include <string>
#include <utility>
#include <stdint.h>

std::pair<RtAudio::DeviceInfo, bool> findDeviceInfo(std::string name, bool contains=true);
std::pair<RtAudio::DeviceInfo, bool> findDeviceInfo(uint32_t deviceID);
std::pair<uint32_t, bool> findDeviceID(std::string name, bool contains=true);

void printDevices();
void printDevice(uint32_t deviceID);

#endif // AUDIOUTIL_HPP
