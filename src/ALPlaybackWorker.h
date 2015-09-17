#pragma once

#include <v8.h>
#include <node.h>
#include <nan.h>
#include <iostream>
#include <queue>

#include <AL/al.h>
#include <AL/alc.h>

#include "Common.h"

using namespace v8;
using namespace Nan;
using namespace std;

#define PLAYBACK_SAMPLE_RATE	48000
#define PLAYBACK_SIZE			2048
#define PLAYBACK_NUM_BUF		16

class ALPlaybackData {
public:
	ALPlaybackData(const char* _data, size_t _size) {
		size = _size;
		data = (char*)malloc(_size);
		memcpy(data, _data, _size);
	}

	~ALPlaybackData() {
		free(data);
	}

public:
	char* data;
	size_t size;
};

class ALPlaybackWorker : public AsyncWorker {
public:
	ALPlaybackWorker(Callback *callback, ALCdevice* device, uv_mutex_t* async_lock, queue<ALPlaybackData*>* dataQueue, bool* playing);
	~ALPlaybackWorker();

	void Execute ();

private:
	void RecoverBuffers();
	void EnqueuePendingData();
	void Play();

private:
	ALCdevice*				device;
	ALCcontext* 			audioContext;
	ALuint					playbackBuffers[PLAYBACK_NUM_BUF];
	ALuint					playbackSources[1];

	bool*					playing;
	uv_mutex_t*				async_lock;
	queue<ALPlaybackData*>*	dataQueue;
	queue<ALuint>			bufferQueue;
};