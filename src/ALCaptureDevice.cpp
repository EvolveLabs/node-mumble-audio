#include "ALCaptureDevice.h"
#include <vector>

using namespace std;
using namespace v8;

ALCaptureDevice::ALCaptureDevice(ALCdevice* device) {
	this->device = device;

    // Generate some AL Buffers for streaming
	//alGenBuffers( CAPTURE_NUMBUFFERS, this->buffers );

	// Generate a Source to playback the Buffers
    //alGenSources( 1, &this->source );
}

ALCaptureDevice::~ALCaptureDevice() {
	if(this->device != NULL) {
		alcCloseDevice(this->device);
	}
}

unsigned int ALCaptureDevice::getAlcSourceId() {
	return 0;
}

NAN_METHOD(ALCaptureDevice::New) {
	NanScope();

	// Get Default audio device
	auto captureDeviceName = alcGetString(NULL, ALC_CAPTURE_DEFAULT_DEVICE_SPECIFIER);

	// Open capture device
	auto device = alcCaptureOpenDevice(captureDeviceName, 48000, AL_FORMAT_MONO16, CAPTURE_BUFFERSIZE);

	ALCaptureDevice* obj = new ALCaptureDevice(device);
	obj->Wrap(args.This());
	NanReturnValue(args.This());
}