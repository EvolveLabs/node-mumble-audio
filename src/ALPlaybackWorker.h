#pragma once

#include <node.h>
#include <node_buffer.h>
#include <nan.h>
#include <v8.h>
#include <iostream>
#include <queue>

#include <AL/al.h>
#include <AL/alc.h>

#include "ALThreading.h"

using namespace v8;
using namespace node;
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
		delete data;
	}

public:
	char* data;
	size_t size;
};

class ALPlaybackWorker : public NanAsyncWorker {
public:
	ALPlaybackWorker(NanCallback *callback, ALCdevice* device, uv_mutex_t* async_lock, queue<ALPlaybackData*>* dataQueue);
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
	ALuint					playbackSource;

	uv_mutex_t*				async_lock;
	queue<ALPlaybackData*>*	dataQueue;
	queue<ALuint>			bufferQueue;
};