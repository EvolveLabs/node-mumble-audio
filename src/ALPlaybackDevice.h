#pragma once
#include <node.h>
#include <node_object_wrap.h>
#include <nan.h>
#include <v8.h>
#include <iostream>
#include <vector>
#include <queue>

#include <AL/al.h>
#include <AL/alc.h>

#include "ALPlaybackWorker.h"

using namespace std;
using namespace node;
using namespace v8;

class ALPlaybackDevice : public ObjectWrap {
	public:
		ALPlaybackDevice::ALPlaybackDevice(ALCdevice* device);
		~ALPlaybackDevice();

		static void Init(Handle<Object> exports);
		
		static NAN_METHOD(GetAll);
		static NAN_METHOD(New);

		static NAN_METHOD(Play);
		static NAN_METHOD(Write);

	private:		
		ALCdevice* 				device;
		uv_mutex_t	 			async_lock;
		queue<ALPlaybackData*> 	buffers;
};
