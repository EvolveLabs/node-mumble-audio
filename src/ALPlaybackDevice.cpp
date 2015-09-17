#include "ALPlaybackDevice.h"

ALPlaybackDevice::ALPlaybackDevice(ALCdevice* _device) : 
	device(_device),
	playing(false)
{	
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
	tpl->PrototypeTemplate()->Set(Nan::New<String>("start").ToLocalChecked(), Nan::New<FunctionTemplate>(Play)->GetFunction());
	tpl->PrototypeTemplate()->Set(Nan::New<String>("stop").ToLocalChecked(), Nan::New<FunctionTemplate>(Stop)->GetFunction());
	tpl->PrototypeTemplate()->Set(Nan::New<String>("write").ToLocalChecked(), Nan::New<FunctionTemplate>(Write)->GetFunction());

	exports->Set(Nan::New<String>("PlaybackDevice").ToLocalChecked(), tpl->GetFunction());
}

// ------------------------------------------
NAN_METHOD(ALPlaybackDevice::New) {

	ALCchar* deviceName = NULL;

	if ( info.Length() > 1)
	{
		return Nan::ThrowTypeError("Invalid number of args.");
	}

	if ( info.Length() == 1)
	{
		if (!info[0]->IsObject())
		{
			return Nan::ThrowTypeError("Invalid argument.");
		} 
		else
		{
			Local<Object> obj = info[0].As<Object>();
			Local<String> name = Nan::Get(obj, Nan::New<String>("name").ToLocalChecked()).ToLocalChecked()->ToString();
			v8::String::Utf8Value val(name);
			deviceName = *val;
		}
	}

	ALCdevice* device = alcOpenDevice(deviceName);
	ALPlaybackDevice* obj = new ALPlaybackDevice(device);
	obj->Wrap( info.This() );
	info.GetReturnValue().Set(info.This());
}

NAN_METHOD(ALPlaybackDevice::Play) {

	auto device = ObjectWrap::Unwrap<ALPlaybackDevice>(info.This());

	if ( device->playing ) {
		return Nan::ThrowTypeError("Already playing.");
	}

	device->playing = true;		
	Nan::AsyncQueueWorker(new ALPlaybackWorker(NULL, device->device, &device->async_lock, &device->buffers, &device->playing));
}

NAN_METHOD(ALPlaybackDevice::Stop) {

	auto device = ObjectWrap::Unwrap<ALPlaybackDevice>(info.This());

	if ( !device->playing ) {
		return Nan::ThrowTypeError("Not playing.");
	}

	device->playing = false;
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