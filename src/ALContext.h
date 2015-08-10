#pragma once
#include <iostream>
#include <vector>
#include <node.h>
#include <node_object_wrap.h>
#include <nan.h>

#ifdef __APPLE__
	#include <OpenAL/al.h>
	#include <OpenAL/alc.h>
#endif

#if defined (_WIN32) || defined (_WIN64)
	#include <AL/al.h>
	#include <AL/alc.h>
#endif

#include "ALDevice.h"

using namespace v8;
using namespace node;
using namespace std;

class ALContext : public ObjectWrap {
	public:
		static void Init(Handle<Object> exports);		

	private:
		ALContext();
		~ALContext();

		static NAN_METHOD(New);
		static NAN_METHOD(SetDevice);
		static NAN_METHOD(SetSource);
		static NAN_METHOD(Play);

	private:
		ALCcontext* context;
		ALDevice* device;
};
