#include "ALPlaybackWorker.h"

ALPlaybackWorker::ALPlaybackWorker(Callback *callback, ALCdevice* _device, uv_mutex_t* _async_lock, queue<ALPlaybackData*>* _dataQueue, bool* _playing) 
: 	AsyncWorker(callback),
	device(_device),
	async_lock(_async_lock),
	dataQueue(_dataQueue),
	playing(_playing)
{ 
	audioContext = alcCreateContext(device, NULL);

	alcMakeContextCurrent(audioContext);

	alGenBuffers(PLAYBACK_NUM_BUF, &playbackBuffers[0]);

	alGenSources(1, &playbackSources[0]);

	for(int i = 0; i < PLAYBACK_NUM_BUF; i++) {
		bufferQueue.push(playbackBuffers[i]);
	}
}
  
ALPlaybackWorker::~ALPlaybackWorker() 
{
	alcMakeContextCurrent(NULL);
	alcDestroyContext(audioContext);
}

void ALPlaybackWorker::RecoverBuffers() 
{
	ALint 	availableBuffers;
	ALuint	unqueuedBuffers[PLAYBACK_NUM_BUF];

	alGetSourcei(playbackSources[0], AL_BUFFERS_PROCESSED, &availableBuffers);
	if(availableBuffers > 0) 
	{
		alSourceUnqueueBuffers(playbackSources[0], availableBuffers, unqueuedBuffers);

		for(int i = 0; i < availableBuffers; i++) 
		{
			bufferQueue.push(unqueuedBuffers[i]);
		}
	}
}

void ALPlaybackWorker::EnqueuePendingData() 
{
	ALPlaybackData* data = NULL;

	uv_mutex_lock(async_lock);
	if (!dataQueue->empty())
	{
		data = dataQueue->front();
		dataQueue->pop();
	}
	uv_mutex_unlock(async_lock);

	if (data != NULL) 
	{
		if (bufferQueue.empty())
		{
			cout << "dropping audio data..." << endl;
		}
		else
		{
			auto b = bufferQueue.front(); 
			bufferQueue.pop();

			alBufferData(b, AL_FORMAT_MONO16, data->data, (ALsizei)data->size, PLAYBACK_SAMPLE_RATE);

			alSourceQueueBuffers(playbackSources[0], 1, &b);
		}

		delete data;
	}
}

void ALPlaybackWorker::Play()
{
	ALint state = 0;
	alGetSourcei(playbackSources[0], AL_SOURCE_STATE, &state);
	if(state != AL_PLAYING) {
		alSourcePlay(playbackSources[0]);
	}
}

// Executed inside the worker-thread.
// It is not safe to access V8, or V8 data structures
// here, so everything we need for input and output
// should go on `this`.
void ALPlaybackWorker::Execute()
{
	while(*this->playing)
	{
		RecoverBuffers();
		EnqueuePendingData();
		Play();
		sleep_ms(1);
	}

	ALint state = 0;
	do
	{
		alGetSourcei(playbackSources[0], AL_SOURCE_STATE, &state);
		sleep_ms(0);
	}
	while(state == AL_PLAYING);
}