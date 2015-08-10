#pragma once
#include <node.h>
#include <node_object_wrap.h>
#include <nan.h>
#include <iostream>
#include <fstream>
#include <cstring>

using namespace v8;
using namespace std;
using namespace node;

class NodeWavData : public ObjectWrap {
	public:
		static void Init(Handle<Object> exports);
		
		char* data;
		int channel;
		int samplerate;
		int bps;
		int size;

	private:
		NodeWavData(const char* path);
		~NodeWavData();

		static NAN_METHOD(New);
};