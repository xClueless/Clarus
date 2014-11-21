#ifndef AUDIOBOUNCER_HPP
#define AUDIOBOUNCER_HPP

#include <clews/sock/SocketConnectionBase.hpp>
#include <RtAudio.h>

class AudioBouncer
{
private:
	SocketConnectionBase* mSocketConnection;
	RtAudio mSenderAudio;
	RtAudio mReceiverAudio;
	RtAudio::StreamParameters mInputParams;
	RtAudio::StreamParameters mOutputParams;
	RtAudio::StreamOptions mStreamOpts;
	uint32_t mInputDevice;
	uint32_t mOutputDevice;
	uint32_t mBufferFrames;
	uint32_t mBufferBytes;
	uint32_t mSampleRate;
	RtAudio::DeviceInfo mInputInfo;
	RtAudio::DeviceInfo mOutputInfo;

	static int sendAudioCallback(void* outputBuffer, void* inputBuffer, unsigned int nFrames, double streamTime, RtAudioStreamStatus status, void* userData);
	static int recieveAudioCallback(void* outputBuffer, void* inputBuffer, unsigned int nFrames, double streamTime, RtAudioStreamStatus status, void* userData);

	int handleAudioSend(void* outputBuffer, void* inputBuffer, unsigned int nFrames, double streamTime, RtAudioStreamStatus status);
	int handleAudioRead(void* outputBuffer, void* inputBuffer, unsigned int nFrames, double streamTime, RtAudioStreamStatus status);

public:
	AudioBouncer(SocketConnectionBase* sc);
	void send();
	void recieve();
};

#endif // AUDIOBOUNCER_HPP
