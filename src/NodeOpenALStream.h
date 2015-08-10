#pragma once
#define NUM_BUFFERS 10

#include <node.h>
#include <node_object_wrap.h>
#include <nan.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>

#ifdef __APPLE__
	#include <OpenAL/al.h>
	#include <OpenAL/alc.h>
#endif

#if defined (_WIN32) || defined (_WIN64)
	#include <AL/al.h>
	#include <AL/alc.h>
    #include <io.h>
#else
    #include <unistd.h>
#endif

using namespace v8;
using namespace std;
using namespace node;

// http://kcat.strangesoft.net/openal-tutorial.html
class NodeOpenALStream : public ObjectWrap {
    public:
        static void Init(Handle<Object> exports);

        void buffer(size_t size, char* bufferdata);
        void setPosition(double x, double y, double z);
        void play();
        bool ready();
        void setGain(float g);

		/* These are what we'll use for OpenAL playback */
		ALuint sourceid, buffers[NUM_BUFFERS];
		ALuint frequency;
		ALenum format;

		int n;
    
    private:
        static NAN_METHOD(New);
        static NAN_METHOD(Buffer);
        static NAN_METHOD(Ready);
        static NAN_METHOD(SetPosition);
        static NAN_METHOD(GetPosition);
        static NAN_METHOD(SetGain);

		NodeOpenALStream(int channels, int bps, int _frequency);
    	~NodeOpenALStream();
};