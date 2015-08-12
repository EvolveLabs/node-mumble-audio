#include "ALCaptureWorker.h"

ALCaptureWorker::ALCaptureWorker(NanCallback *callback, ALCdevice* _device) 
: NanAsyncProgressWorker(callback),
	device(_device)
{
}
  
ALCaptureWorker::~ALCaptureWorker() 
{
	alcCaptureStop(device);
}

// Executed inside the worker-thread.
// It is not safe to access V8, or V8 data structures
// here, so everything we need for input and output
// should go on `this`.
void ALCaptureWorker::Execute(const ExecutionProgress& progress)
{
	ALCint			samplesIn = 0;

	alcCaptureStart(device);

	while(true) {

		alcGetIntegerv(device, ALC_CAPTURE_SAMPLES, 1, &samplesIn);

		if (samplesIn > CAPTURE_SIZE) {

			int sampleSize = CAPTURE_SIZE * sizeof(short);

			char* captured = (char*)malloc(sampleSize);

			alcCaptureSamples(device, captured, CAPTURE_SIZE);

			progress.Send(captured, sampleSize);
		}

		sleep(0);
	}
}

void ALCaptureWorker::HandleProgressCallback(const char* data, size_t size)
{
	NanScope();

	Local<Value> args[] = {
		NanNewBufferHandle((char*)data, (uint32_t)size)
	};
    callback->Call(1, args);
}