#include "ALCaptureDevice.h"
#include <vector>

using namespace std;
using namespace v8;

ALCaptureDevice::ALCaptureDevice(ALCdevice* _device) 
	: device(_device)
{
}

ALCaptureDevice::~ALCaptureDevice() {
	alcCaptureCloseDevice(device);
}

void ALCaptureDevice::Init(Handle<Object> exports) {
	// Prepare constructor template
	Local<FunctionTemplate> tpl = NanNew<FunctionTemplate>(New);
	tpl->SetClassName(NanNew<String>("CaptureDevice"));
	tpl->InstanceTemplate()->SetInternalFieldCount(1);

	tpl->PrototypeTemplate()->Set(NanNew<String>("on"), NanNew<FunctionTemplate>(On)->GetFunction());
	tpl->PrototypeTemplate()->Set(NanNew<String>("start"), NanNew<FunctionTemplate>(Start)->GetFunction());

	exports->Set(NanNew<String>("CaptureDevice"), tpl->GetFunction());
}

NAN_METHOD(ALCaptureDevice::New) {
	NanScope();

	// Get Default audio device
	auto captureDeviceName = alcGetString(NULL, ALC_CAPTURE_DEFAULT_DEVICE_SPECIFIER);

	// Open capture device
	auto device = alcCaptureOpenDevice(captureDeviceName, CAPTURE_SAMPLE_RATE, AL_FORMAT_MONO16, CAPTURE_SAMPLE_RATE / 2);

	ALCaptureDevice* obj = new ALCaptureDevice(device);
	obj->Wrap(args.This());
	NanReturnValue(args.This());
}

NAN_METHOD(ALCaptureDevice::On) {
	NanScope();

	if (args.Length() != 2 || !args[0]->IsString() || !args[1]->IsFunction()) {
		NanThrowTypeError("Invalid args, expected: (string, function)");
		NanReturnUndefined();
	}

	auto device = ObjectWrap::Unwrap<ALCaptureDevice>(args.This());

	auto arg0 = args[0].As<String>();
	// todo: switch on arg0 value...

	device->onData = new NanCallback(args[1].As<Function>());

}

NAN_METHOD(ALCaptureDevice::Start) {
	NanScope();
	auto device = ObjectWrap::Unwrap<ALCaptureDevice>(args.This());
	NanAsyncQueueWorker(new ALCaptureWorker(device->onData, device->device));
	NanReturnUndefined();
}