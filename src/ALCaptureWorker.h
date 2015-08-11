#pragma once

#include <node.h>
#include <nan.h>
#include <v8.h>
#include <iostream>

#include <AL/al.h>
#include <AL/alc.h>

using namespace v8;
using namespace node;
using namespace std;

#define CAPTURE_SAMPLE_RATE	48000
#define CAPTURE_SIZE		2048

class ALCaptureWorker : public NanAsyncProgressWorker {
public:
	ALCaptureWorker(NanCallback *callback, ALCdevice* device);
	~ALCaptureWorker();

	void Execute (const ExecutionProgress& progress);
	void HandleProgressCallback(const char *,size_t);
	void HandleOKCallback() override { }

private:
	ALCdevice*		device;
};