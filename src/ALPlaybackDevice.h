#pragma once
#include <v8.h>
#include <node.h>
#include <nan.h>
#include <iostream>
#include <vector>
#include <queue>

#include <AL/al.h>
#include <AL/alc.h>

#include "ALPlaybackWorker.h"

using namespace std;
using namespace Nan;
using namespace v8;

class ALPlaybackDevice : public ObjectWrap {
	public:
		ALPlaybackDevice(ALCdevice* device);
		~ALPlaybackDevice();

		static void Init(Handle<Object> exports);

		static NAN_METHOD(New);
		static NAN_METHOD(Play);
		static NAN_METHOD(Stop);
		static NAN_METHOD(Write);

	private:		
		ALCdevice* 				device;
		uv_mutex_t	 			async_lock;
		queue<ALPlaybackData*> 	buffers;
		bool					playing;					
};
