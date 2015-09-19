#include "ALDeviceEnumeration.h"

ALDeviceEnumeration::ALDeviceEnumeration() {	
};

// ------------------------------------------
ALDeviceEnumeration::~ALDeviceEnumeration() {
};

// ------------------------------------------
void ALDeviceEnumeration::Init(Handle<Object> exports) {
	exports->Set(Nan::New<String>("devices").ToLocalChecked(), Nan::New<FunctionTemplate>(Devices)->GetFunction());
}

NAN_METHOD(ALDeviceEnumeration::Devices) {

	if(!alcIsExtensionPresent( NULL, "ALC_ENUMERATION_EXT" )) {
		return Nan::ThrowError("Enumeration extension is not present.");
	}

	if(!alcIsExtensionPresent( NULL, "ALC_ENUMERATE_ALL_EXT" )) {
		return Nan::ThrowError("Enumeration extension is not present.");
	}

#if _WIN32
	const ALchar* defaultDevice = alcGetString( NULL, ALC_DEFAULT_ALL_DEVICES_SPECIFIER );
	const ALchar* defaultCaptureDevice = alcGetString( NULL, ALC_CAPTURE_DEFAULT_DEVICE_SPECIFIER );
#else
	// On darwin, for some reason, the default names are duplicated at the front of the names lists below.
	// Set the default names to null and pull them off of the front of the list instead.
	const ALchar* defaultDevice = NULL;
	const ALchar* defaultCaptureDevice = NULL;
#endif

	const ALchar* deviceNames = alcGetString( NULL, ALC_ALL_DEVICES_SPECIFIER );
	const ALchar* captureDeviceNames = alcGetString( NULL, ALC_CAPTURE_DEVICE_SPECIFIER );

	Local<Object> results = Nan::New<Array>(0);

	// The device names strings are actually \0 seperated list of device names. The list ends with an empty string.
	// The first name in each list is the default device.
	// Unless there is only one device, in that case than it's both the default and the only device.

	int i = 0;
	int deviceCount = 0;
	int captureDeviceCount = 0;
	while(deviceNames && *deviceNames)
	{
		if (defaultDevice == NULL)
		{
			defaultDevice = deviceNames;
		}
		else
		{
			Local<Object> obj = Nan::New<Object>();
			obj->Set(Nan::New<String>("name").ToLocalChecked(), Nan::New<String>(deviceNames).ToLocalChecked());
			obj->Set(Nan::New<String>("kind").ToLocalChecked(), Nan::New<String>("output").ToLocalChecked());
			if(strcmp(deviceNames, defaultDevice) == 0)
			{
				obj->Set(Nan::New<String>("default").ToLocalChecked(), Nan::New<Boolean>(true));
			}

			results->Set(i, obj);

			i++;
			deviceCount++;
		}

		deviceNames += strlen(deviceNames) + 1;
	}

	while(captureDeviceNames && *captureDeviceNames) 
	{
		if (defaultCaptureDevice == NULL)
		{
			defaultCaptureDevice = captureDeviceNames;
		}
		else
		{
			Local<Object> obj = Nan::New<Object>();
			obj->Set(Nan::New<String>("name").ToLocalChecked(), Nan::New<String>(captureDeviceNames).ToLocalChecked());
			obj->Set(Nan::New<String>("kind").ToLocalChecked(), Nan::New<String>("capture").ToLocalChecked());
			if(strcmp(captureDeviceNames, defaultCaptureDevice) == 0)
			{
				obj->Set(Nan::New<String>("default").ToLocalChecked(), Nan::New<Boolean>(true));
			}

			results->Set(i, obj);

			i++;
			captureDeviceCount++;
		}

		captureDeviceNames += strlen(captureDeviceNames) + 1;
	}

	if (deviceCount == 0 && defaultDevice != NULL)
	{
		Local<Object> obj = Nan::New<Object>();
		obj->Set(Nan::New<String>("name").ToLocalChecked(), Nan::New<String>(defaultDevice).ToLocalChecked());
		obj->Set(Nan::New<String>("kind").ToLocalChecked(), Nan::New<String>("output").ToLocalChecked());
		obj->Set(Nan::New<String>("default").ToLocalChecked(), Nan::New<Boolean>(true));
		results->Set(i++, obj);
	}

	if (captureDeviceCount == 0 && defaultCaptureDevice !=  NULL)
	{
		Local<Object> obj = Nan::New<Object>();
		obj->Set(Nan::New<String>("name").ToLocalChecked(), Nan::New<String>(defaultCaptureDevice).ToLocalChecked());
		obj->Set(Nan::New<String>("kind").ToLocalChecked(), Nan::New<String>("capture").ToLocalChecked());
		obj->Set(Nan::New<String>("default").ToLocalChecked(), Nan::New<Boolean>(true));
		results->Set(i++, obj);
	}

	info.GetReturnValue().Set(results);
}