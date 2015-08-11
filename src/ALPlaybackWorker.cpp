#include "ALPlaybackWorker.h"
#include "ALThreading.h"

ALPlaybackWorker::ALPlaybackWorker(NanCallback *callback, ALCdevice* _device, uv_mutex_t* _async_lock, queue<ALPlaybackData*>* _buffers) 
: NanAsyncWorker(callback),
	device(_device),
	async_lock(_async_lock),
	buffers(_buffers)
{
}
  
ALPlaybackWorker::~ALPlaybackWorker() 
{
}

// Executed inside the worker-thread.
// It is not safe to access V8, or V8 data structures
// here, so everything we need for input and output
// should go on `this`.
void ALPlaybackWorker::Execute()
{
	while(true)
	{
		ALPlaybackData* data = NULL;

		uv_mutex_lock(async_lock);
		if (!buffers->empty())
		{
			data = buffers->front();
			buffers->pop();
		}
		uv_mutex_unlock(async_lock);

		if (data != NULL) 
		{
			cout << "processing data..." << endl;
			delete data;
		}

		sleep(0);
	}
}