#include "ALPlaybackDevice.h"

ALPlaybackDevice::ALPlaybackDevice(ALCdevice* _device) : device(_device) {	
    uv_mutex_init(&async_lock);
};

// ------------------------------------------
ALPlaybackDevice::~ALPlaybackDevice() {

    uv_mutex_destroy(&async_lock);

	alcCloseDevice(device);
};

// ------------------------------------------
void ALPlaybackDevice::Init(Handle<Object> exports) {
	// Prepare constructor template
	Local<FunctionTemplate> tpl = Nan::New<FunctionTemplate>(New);
	tpl->SetClassName(Nan::New<String>("Device").ToLocalChecked());
	tpl->InstanceTemplate()->SetInternalFieldCount(1);

	// Static functions on Device
	tpl->Set(Nan::New<String>("GetAll").ToLocalChecked(), Nan::New<FunctionTemplate>(GetAll)->GetFunction());

	tpl->PrototypeTemplate()->Set(Nan::New<String>("play").ToLocalChecked(), Nan::New<FunctionTemplate>(Play)->GetFunction());
	tpl->PrototypeTemplate()->Set(Nan::New<String>("write").ToLocalChecked(), Nan::New<FunctionTemplate>(Write)->GetFunction());

	exports->Set(Nan::New<String>("PlaybackDevice").ToLocalChecked(), tpl->GetFunction());
}

// ------------------------------------------
NAN_METHOD(ALPlaybackDevice::New) {
	ALCdevice* device = alcOpenDevice(NULL);
	ALPlaybackDevice* obj = new ALPlaybackDevice(device);
	obj->Wrap( info.This() );
	info.GetReturnValue().Set(info.This());
}

NAN_METHOD(ALPlaybackDevice::GetAll) {

	if(!alcIsExtensionPresent( NULL, "ALC_ENUMERATION_EXT" )) {
		return Nan::ThrowError("Enumeration extension is not present.");
	}

	if(!alcIsExtensionPresent( NULL, "ALC_ENUMERATE_ALL_EXT" )) {
		return Nan::ThrowError("Enumeration extension is not present.");
	}

	const ALchar* defaultDevice = alcGetString( NULL, ALC_DEFAULT_ALL_DEVICES_SPECIFIER );
	const ALchar* deviceNames = alcGetString( NULL, ALC_ALL_DEVICES_SPECIFIER );

	const ALchar* defaultCaptureDevice = alcGetString( NULL, ALC_CAPTURE_DEFAULT_DEVICE_SPECIFIER);
	const ALchar* captureDeviceNames = alcGetString( NULL, ALC_CAPTURE_DEVICE_SPECIFIER );

	vector< ALCdevice* > devices;
	vector< ALCdevice* > captureDevices;

	while(deviceNames && *deviceNames) {
		ALCdevice* device = alcOpenDevice(deviceNames);
		devices.push_back(device);
		deviceNames += strlen(deviceNames) + 1;
	}

	while(captureDeviceNames && *captureDeviceNames) {
		ALCdevice* device = alcOpenDevice(captureDeviceNames);
		captureDevices.push_back(device);
		captureDeviceNames += strlen(captureDeviceNames) + 1;
	}

	int deviceCount = (int)devices.size() + (int)captureDevices.size();
	Local<Array> results = Nan::New<Array>(deviceCount);
	for(int i = 0; i < devices.size(); i++) {
		ALCdevice* device = devices.at(i);
		Local<Object> obj = Nan::New<Object>();

		const ALchar* name = alcGetString( device, ALC_DEVICE_SPECIFIER );

		obj->Set(Nan::New<String>("name").ToLocalChecked(), Nan::New<String>(name).ToLocalChecked());
		obj->Set(Nan::New<String>("kind").ToLocalChecked(), Nan::New<String>("output").ToLocalChecked());
		if(strcmp(name, defaultDevice) == 0) {
			obj->Set(Nan::New<String>("default").ToLocalChecked(), Nan::New<Boolean>(true));
		}

		results->Set(i, obj);
		alcCloseDevice( device );
	}

	for(int i = 0; i < captureDevices.size(); i++) {
		ALCdevice* device = captureDevices.at(i);
		Local<Object> obj = Nan::New<Object>();

		const ALchar* captureName = alcGetString( device, ALC_CAPTURE_DEVICE_SPECIFIER );
		
		obj->Set(Nan::New<String>("name").ToLocalChecked(), Nan::New<String>(captureName).ToLocalChecked());
		obj->Set(Nan::New<String>("kind").ToLocalChecked(), Nan::New<String>("capture").ToLocalChecked());
		if(strcmp(captureName, defaultCaptureDevice) == 0) {
			obj->Set(Nan::New<String>("default").ToLocalChecked(), Nan::New<Boolean>(true));
		}

		results->Set((int)devices.size() + i, obj);
		alcCloseDevice( device );
	}

	info.GetReturnValue().Set(results);
}

NAN_METHOD(ALPlaybackDevice::Play) {
	auto device = ObjectWrap::Unwrap<ALPlaybackDevice>(info.This());
	Nan::AsyncQueueWorker(new ALPlaybackWorker(NULL, device->device, &device->async_lock, &device->buffers));
}

NAN_METHOD(ALPlaybackDevice::Write) {

	auto device = ObjectWrap::Unwrap<ALPlaybackDevice>(info.This());

	Local<Object> val = info[0].As<Object>();
	auto buffer = node::Buffer::Data(val);
	auto len = node::Buffer::Length(val);
	auto data = new ALPlaybackData(buffer, len);

    uv_mutex_lock(&device->async_lock);
    device->buffers.push(data);
    uv_mutex_unlock(&device->async_lock);
}