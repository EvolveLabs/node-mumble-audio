#pragma once
#include <iostream>
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

#include "NodeWavData.h"

using namespace v8;
using namespace node;
using namespace std;

class NodeOpenALSource : public ObjectWrap {
	public:
		static void Init(Handle<Object> exports);

	private:
		NodeOpenALSource( NodeWavData* data );
		~NodeOpenALSource();

		static NAN_METHOD(New);
		static NAN_METHOD(Play);
		static NAN_METHOD(SetPosition);
		static NAN_METHOD(SetLoop);

		void play();
		void setPosition(double x, double y, double z);
		void setLoop(bool loop);
		unsigned int bufferid;
		unsigned int format;
		unsigned int sourceid;
};
