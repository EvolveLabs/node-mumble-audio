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

using namespace std;
using namespace node;
using namespace v8;

class ALDevice : public ObjectWrap {
	public:
		ALDevice::ALDevice(ALCdevice* device);
		~ALDevice();

		static void Init(Handle<Object> exports);
		
		static NAN_METHOD(GetAll);
		static NAN_METHOD(New);
		
		ALCdevice* getAlcDevice();

	private:		
		ALCdevice* device;
};
