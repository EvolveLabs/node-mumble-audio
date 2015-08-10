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

class NodeOpenALDevice : public ObjectWrap {
	public:
		static void Init(Handle<Object> exports);
		ALCdevice* device;

		//static vector<NodeOpenALDevice*> devices;

	private:
		NodeOpenALDevice();
		~NodeOpenALDevice();

		static NAN_METHOD(New);
};
