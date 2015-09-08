#include "ALCaptureDevice.h"

ALCaptureDevice::ALCaptureDevice(ALCdevice* _device) 
	: device(_device)
{
}

ALCaptureDevice::~ALCaptureDevice() {
	alcCaptureCloseDevice(device);
}

void ALCaptureDevice::Init(Handle<Object> exports) {
	// Prepare constructor template
	Local<FunctionTemplate> tpl = Nan::New<FunctionTemplate>(New);
	tpl->SetClassName(Nan::New<String>("CaptureDevice").ToLocalChecked());
	tpl->InstanceTemplate()->SetInternalFieldCount(1);

	tpl->PrototypeTemplate()->Set(Nan::New<String>("on").ToLocalChecked(), Nan::New<FunctionTemplate>(On)->GetFunction());
	tpl->PrototypeTemplate()->Set(Nan::New<String>("start").ToLocalChecked(), Nan::New<FunctionTemplate>(Start)->GetFunction());

	exports->Set(Nan::New<String>("CaptureDevice").ToLocalChecked(), tpl->GetFunction());
}

NAN_METHOD(ALCaptureDevice::New) {

	// Get Default audio device
	auto captureDeviceName = alcGetString(NULL, ALC_CAPTURE_DEFAULT_DEVICE_SPECIFIER);

	// Open capture device
	auto device = alcCaptureOpenDevice(captureDeviceName, CAPTURE_SAMPLE_RATE, AL_FORMAT_MONO16, CAPTURE_SAMPLE_RATE / 2);

	ALCaptureDevice* obj = new ALCaptureDevice(device);
	obj->Wrap(info.This());
	info.GetReturnValue().Set(info.This());
}

NAN_METHOD(ALCaptureDevice::On) {

	if (info.Length() != 2 || !info[0]->IsString() || !info[1]->IsFunction()) {
		return Nan::ThrowTypeError("Invalid args, expected: (string, function)");
	}

	auto device = ObjectWrap::Unwrap<ALCaptureDevice>(info.This());

	// auto arg0 = info[0].As<String>();
	// todo: switch on arg0 value...

	device->onData = new Callback(info[1].As<Function>());

}

NAN_METHOD(ALCaptureDevice::Start) {
	auto device = ObjectWrap::Unwrap<ALCaptureDevice>(info.This());
	Nan::AsyncQueueWorker(new ALCaptureWorker(device->onData, device->device));
}