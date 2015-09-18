#include "ALCaptureWorker.h"

ALCaptureWorker::ALCaptureWorker(Callback *callback, ALCdevice* _device, bool* _capturing) : 
	AsyncProgressWorker(callback),
	device(_device),
	capturing(_capturing)
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

	while(*this->capturing)
	{
		alcGetIntegerv(device, ALC_CAPTURE_SAMPLES, 1, &samplesIn);

		if (samplesIn > CAPTURE_SIZE) {

			int sampleSize = CAPTURE_SIZE * sizeof(short);

			char* captured = (char*)malloc(sampleSize);

			alcCaptureSamples(device, captured, CAPTURE_SIZE);

			if (*this->capturing)
			{
				progress.Send(captured, sampleSize);
			}

			delete captured;
		}

		sleep_ms(1);
	}
}

void ALCaptureWorker::HandleProgressCallback(const char* data, size_t size)
{
	Nan::HandleScope scope;
	if (*this->capturing)
	{
		Local<Value> args[] =
		{
			Nan::CopyBuffer((char*)data, (uint32_t)size).ToLocalChecked()
		};
	    callback->Call(1, args);
	}
}
