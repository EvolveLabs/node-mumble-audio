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

#include "NodeOpenALDevice.h"

using namespace v8;
using namespace node;
using namespace std;

class NodeOpenALContext : public ObjectWrap {
	public:
		static void Init(Handle<Object> exports);
		ALCcontext* context;
		//static vector<NodeOpenALContext*> contexts;

	private:
		NodeOpenALContext(NodeOpenALDevice* dev);
		~NodeOpenALContext();

		static NAN_METHOD(New);
		static NAN_METHOD(PlusOne);
};
