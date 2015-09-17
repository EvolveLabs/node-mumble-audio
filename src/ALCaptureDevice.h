#pragma once
#include <v8.h>
#include <node.h>
#include <nan.h>
#include <iostream>

#include <AL/al.h>
#include <AL/alc.h>

#include "ALCaptureWorker.h"

using namespace v8;
using namespace Nan;
using namespace std;

class ALCaptureDevice : public ObjectWrap
{
	public:
		ALCaptureDevice(ALCdevice* device);
		~ALCaptureDevice();

		static void Init(Handle<Object> exports);

		static NAN_METHOD(New);
		static NAN_METHOD(Start);
		static NAN_METHOD(Stop);
		static NAN_METHOD(On);

	private:
		ALCdevice*			device;
		Callback*			onData;
		bool				capturing;
};
