#ifndef AUDIOUTIL_HPP
#define AUDIOUTIL_HPP

#include <RtAudio.h>
#include <string>
#include <experimental/optional>

std::experimental::optional<RtAudio::DeviceInfo> findDeviceInfo(std::string name, bool contains=true);
std::experimental::optional<RtAudio::DeviceInfo> findDeviceInfo(uint32_t deviceID);
std::experimental::optional<uint32_t> findDeviceID(std::string name, bool contains=true);

void printDevices();
void printDevice(uint32_t deviceID);

#endif // AUDIOUTIL_HPP
