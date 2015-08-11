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

#define CAPTURE_SAMPLE_RATE	48000
#define CAPTURE_SIZE		2048

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

private:
	char* data;
	size_t size;
};

class ALPlaybackWorker : public NanAsyncWorker {
public:
	ALPlaybackWorker(NanCallback *callback, ALCdevice* device, uv_mutex_t* async_lock, queue<ALPlaybackData*>* buffers);
	~ALPlaybackWorker();

	void Execute ();

private:
	ALCdevice*				device;
	uv_mutex_t*				async_lock;
	queue<ALPlaybackData*>*	buffers;
};