#include "AudioBouncer.hpp"

#include <iostream>
#include <cstring>

using namespace std;

AudioBouncer::AudioBouncer(SocketConnectionBase* sc) : mSocketConnection(sc), mSenderAudio(RtAudio::LINUX_PULSE), mReceiverAudio(RtAudio::LINUX_PULSE)
{
	RtAudio::Api currentAPI = mSenderAudio.getCurrentApi();
	if(currentAPI != RtAudio::LINUX_PULSE)
	{
		cout << "NOT USING PULSE!" << endl;
	}

	mInputDevice = mReceiverAudio.getDefaultInputDevice();
	mOutputDevice = mSenderAudio.getDefaultOutputDevice();

	mInputInfo = mSenderAudio.getDeviceInfo(mInputDevice);
	mOutputInfo = mSenderAudio.getDeviceInfo(mOutputDevice);

	mInputParams.deviceId = mInputDevice;
	mInputParams.firstChannel = 0;
	mInputParams.nChannels = 2;

	mOutputParams.deviceId = mOutputDevice;
	mOutputParams.firstChannel = 0;
	mOutputParams.nChannels = 2;

	mStreamOpts.numberOfBuffers = 5;
	mStreamOpts.streamName = "Clarus";

	mBufferFrames = 1024;
	mBufferBytes = 1024;
	mSampleRate = mInputInfo.sampleRates[mInputInfo.sampleRates.size()-1];
}

int AudioBouncer::sendAudioCallback(void* outputBuffer, void* inputBuffer, unsigned int nFrames, double streamTime, RtAudioStreamStatus status, void* userData)
{
	cout << "Send callback triggered" << endl;

	AudioBouncer* instance = (AudioBouncer*) userData;
	return instance->handleAudioSend(outputBuffer, inputBuffer, nFrames, streamTime, status);
}

int AudioBouncer::recieveAudioCallback(void* outputBuffer, void* inputBuffer, unsigned int nFrames, double streamTime, RtAudioStreamStatus status, void* userData)
{
	cout << "Recieve callback triggered" << endl;

	AudioBouncer* instance = (AudioBouncer*) userData;
	return instance->handleAudioRead(outputBuffer, inputBuffer, nFrames, streamTime, status);
}

int AudioBouncer::handleAudioRead(void* outputBuffer, void* inputBuffer, unsigned int nFrames, double streamTime, RtAudioStreamStatus status)
{
	vector<char> audioData = mSocketConnection->read();
	memcpy(outputBuffer, &audioData.at(0), audioData.size());
	cout << "Read " << audioData.size() << " bytes of audio" << endl;

	return 0;
}

int AudioBouncer::handleAudioSend(void* outputBuffer, void* inputBuffer, unsigned int nFrames, double streamTime, RtAudioStreamStatus status)
{
	vector<char> audioBuffer(mBufferBytes);
	memcpy(&audioBuffer.at(0), inputBuffer, mBufferBytes);
	mSocketConnection->write(audioBuffer);
	cout << "Sent " << audioBuffer.size() << " bytes of audio" << endl;

	return 0;
}

void AudioBouncer::send()
{
	mSenderAudio.openStream(NULL, &mInputParams, RTAUDIO_FLOAT32, mSampleRate, &mBufferFrames, sendAudioCallback, (void*)this, &mStreamOpts, NULL);

	mBufferBytes = mBufferFrames * 2 * 4;

	mSenderAudio.startStream();
}

void AudioBouncer::recieve()
{
	mReceiverAudio.openStream(&mOutputParams, NULL, RTAUDIO_FLOAT32, mSampleRate, &mBufferFrames, recieveAudioCallback, (void*)this, &mStreamOpts, NULL);
	mReceiverAudio.startStream();
}
