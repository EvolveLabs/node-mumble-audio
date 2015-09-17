#pragma once

#include <v8.h>
#include <node.h>
#include <nan.h>
#include <iostream>

#include <AL/al.h>
#include <AL/alc.h>

#include "Common.h"

using namespace v8;
using namespace Nan;
using namespace std;

#define CAPTURE_SAMPLE_RATE	48000
#define CAPTURE_SIZE		2048

class ALCaptureWorker : public AsyncProgressWorker {
public:
	ALCaptureWorker(Callback *callback, ALCdevice* device, bool* capturing);
	~ALCaptureWorker();

	void Execute (const ExecutionProgress& progress);
	void HandleProgressCallback(const char *,size_t);
	void HandleOKCallback() override { }

private:
	ALCdevice*		device;
	bool*			capturing;
};