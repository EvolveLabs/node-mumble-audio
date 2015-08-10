#pragma once
#include <iostream>
#include <vector>
#include <node.h>
#include <node_object_wrap.h>
#include <nan.h>
#include <v8.h>

#ifdef __APPLE__
	#include <OpenAL/al.h>
	#include <OpenAL/alc.h>
#endif

#if defined (_WIN32) || defined (_WIN64)
	#include <AL/al.h>
	#include <AL/alc.h>
#endif

#include "IContextSource.h"

#define CAPTURE_BUFFERSIZE	4410
#define CAPTURE_NUMBUFFERS	4

using namespace std;
using namespace node;
using namespace v8;

class ALCaptureDevice :  
	public ObjectWrap,
	public IContextSource
{
	public:
		ALCaptureDevice(ALCdevice* device);
		~ALCaptureDevice();

		virtual unsigned int getAlcSourceId();

		static NAN_METHOD(New);

	private:
		ALCdevice*	device;
		ALuint		source;
		ALuint		buffers[CAPTURE_NUMBUFFERS];
		ALuint		buffer;
};
