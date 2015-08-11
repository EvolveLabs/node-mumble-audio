#include "ALCaptureWorker.h"
#include "ALThreading.h"

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
	char			buffer[CAPTURE_SAMPLE_RATE * 2];
	ALCint			samplesIn = 0;

	alcCaptureStart(device);

	while(true) {

		alcGetIntegerv(device, ALC_CAPTURE_SAMPLES, 1, &samplesIn);

		if (samplesIn > CAPTURE_SIZE) {

			alcCaptureSamples(device, buffer, CAPTURE_SIZE);

			progress.Send(buffer, sizeof(buffer));
		}

		sleep(0);
	}
}

void ALCaptureWorker::HandleProgressCallback(const char *data, size_t size)
{
	NanScope();

	Local<Value> args[] = {
		NanNewBufferHandle(data, (uint32_t)size)
	};
    callback->Call(1, args);
}