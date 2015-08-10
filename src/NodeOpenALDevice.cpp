#include <node.h>
#include "NodeOpenAlDevice.h"
#include <vector>

using namespace v8;

//vector<NodeOpenALDevice*> NodeOpenALDevice::devices;

// ------------------------------------------
NodeOpenALDevice::NodeOpenALDevice() {
	device = alcOpenDevice(NULL);
    if(device==NULL) {
		std::cout << "cannot open sound card" << std::endl;
		return;
    }
};

// ------------------------------------------
NodeOpenALDevice::~NodeOpenALDevice() {
	if(device) {
		cout << "destroying device" << endl;
		alcCloseDevice(device);
	}
};

// ------------------------------------------
void NodeOpenALDevice::Init(Handle<Object> exports) {
	// Prepare constructor template
	Local<FunctionTemplate> tpl = NanNew<FunctionTemplate>(New);
	tpl->SetClassName(NanNew<String>("Device"));
	tpl->InstanceTemplate()->SetInternalFieldCount(1);

	// Static functions on Device
	tpl->Set(NanNew<String>("GetAll"), NanNew<FunctionTemplate>(GetAll)->GetFunction());

	exports->Set(NanNew<String>("Device"), tpl->GetFunction());
}

// ------------------------------------------
NAN_METHOD(NodeOpenALDevice::New) {
	NanScope();

	NodeOpenALDevice* obj = new NodeOpenALDevice();
	//devices.push_back( obj );
	obj->Wrap( args.This() );
	NanReturnValue(args.This());
}

NAN_METHOD(NodeOpenALDevice::GetAll) {
	NanScope();

	if(!alcIsExtensionPresent( NULL, "ALC_ENUMERATION_EXT" )) {
		NanThrowError("Enumeration extension is not present.");
		NanReturnUndefined();
	}

	if(!alcIsExtensionPresent( NULL, "ALC_ENUMERATE_ALL_EXT" )) {
		NanThrowError("Enumeration extension is not present.");
		NanReturnUndefined();
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
	Local<Array> results = NanNew<Array>(deviceCount);
	for(int i = 0; i < devices.size(); i++) {
		ALCdevice* device = devices.at(i);
		Local<Object> obj = NanNew<Object>();

		const ALchar* name = alcGetString( device, ALC_DEVICE_SPECIFIER );

		obj->Set(NanNew<String>("name"), NanNew<String>(name));
		obj->Set(NanNew<String>("kind"), NanNew<String>("output"));
		if(strcmp(name, defaultDevice) == 0) {
			obj->Set(NanNew<String>("default"), NanNew<Boolean>(true));
		}

		results->Set(i, obj);
		alcCloseDevice( device );
	}

	for(int i = 0; i < captureDevices.size(); i++) {
		ALCdevice* device = captureDevices.at(i);
		Local<Object> obj = NanNew<Object>();

		const ALchar* captureName = alcGetString( device, ALC_CAPTURE_DEVICE_SPECIFIER );
		
		obj->Set(NanNew<String>("name"), NanNew<String>(captureName));
		obj->Set(NanNew<String>("kind"), NanNew<String>("capture"));
		if(strcmp(captureName, defaultCaptureDevice) == 0) {
			obj->Set(NanNew<String>("default"), NanNew<Boolean>(true));
		}

		results->Set((int)devices.size() + i, obj);
		alcCloseDevice( device );
	}

	NanReturnValue(results);
}