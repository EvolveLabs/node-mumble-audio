#include <node.h>
#include <nan.h>
#include <v8.h>
#include <delay_load_hook.h>

#ifdef __APPLE__
	#include <OpenAL/al.h>
	#include <OpenAL/alc.h>
#endif

#if defined (_WIN32) || defined (_WIN64)
	#include <AL/al.h>
	#include <AL/alc.h>
#endif

#include "NodeWavData.h"
#include "NodeOpenALDevice.h"
#include "NodeOpenALContext.h"
#include "NodeOpenALSource.h"
#include "NodeOpenALStream.h"

using namespace v8;
using namespace std;

void PrintOpenALInfo()
{
	if (alGetString(AL_VERSION))
		cout << "OpenAL version: "    << alGetString(AL_VERSION)    << endl;
	if (alGetString(AL_RENDERER))
		cout << "OpenAL renderer: "   << alGetString(AL_RENDERER)   << endl;
	if (alGetString(AL_VENDOR))
		cout << "OpenAL vendor: "     << alGetString(AL_VENDOR)     << endl;
	if (alGetString(AL_EXTENSIONS))
		cout << "OpenAL extensions: " << alGetString(AL_EXTENSIONS) << endl;

	// Enumerate OpenAL devices
	if (alcIsExtensionPresent (NULL, (const ALCchar *) "ALC_ENUMERATION_EXT") == AL_TRUE)
	{
		const char *s = (const char *) alcGetString(NULL, ALC_DEVICE_SPECIFIER);
		while (*s != '\0')
		{
			cout << "OpenAL available device: " << s << endl;
			while (*s++ != '\0');
		}
	}
	else
	{
		cout << "OpenAL device enumeration isn't available." << endl;
	}

	// Print default device name
	cout << "OpenAL default device: "
	     << (const char *)alcGetString(NULL, ALC_DEFAULT_DEVICE_SPECIFIER)
		 << endl;

	// Print current device name
	// if (myDevice)
	// {
	// 	cout << "OpenAL current device: "
	// 		 << (const char *)alcGetString(myDevice, ALC_DEVICE_SPECIFIER)
	// 		 << endl;
	// }
}



// --------------------------------------------------
NAN_METHOD(SetListenerOrientation) {
	NanScope();

	double x = args[0]->NumberValue();
	double y = args[1]->NumberValue();
	double z = args[2]->NumberValue();
	float o[] = {x, y, z, 0, 1, 0};
	alListenerfv(AL_ORIENTATION, o);

	NanReturnUndefined();
}

// --------------------------------------------------
NAN_METHOD(SetListenerPosition) {
	NanScope();

	double x = args[0]->NumberValue();
	double y = args[1]->NumberValue();
	double z = args[2]->NumberValue();
	float p[] = {x, y, z, 0, 1, 0};
	alListenerfv(AL_POSITION, p);

	NanReturnUndefined();
}

// --------------------------------------------------
NAN_METHOD(MakeContextCurrent) {
	NanScope();

	if (args.Length() != 1) {
		NanThrowTypeError("Expected a single Context argument.");
		NanReturnUndefined();
	}

	if ( !args[0]->IsObject() ) {
		NanThrowTypeError("Expected a single Context argument.");
		NanReturnUndefined();
	}

	NodeOpenALContext* ctx = node::ObjectWrap::Unwrap<NodeOpenALContext>(args[0]->ToObject());
	alcMakeContextCurrent(ctx->context);

	NanReturnUndefined();
}

// --------------------------------------------------
void NodeInit(Handle<Object> exports) {
	PrintOpenALInfo();
	
	NodeWavData::Init(exports);
	NodeOpenALContext::Init(exports);
	NodeOpenALDevice::Init(exports);
	NodeOpenALSource::Init(exports);
	NodeOpenALStream::Init(exports);
	
	exports->Set( NanNew<String>("MakeContextCurrent"), NanNew<FunctionTemplate>(MakeContextCurrent)->GetFunction() );
	exports->Set( NanNew<String>("SetListenerPosition"), NanNew<FunctionTemplate>(SetListenerPosition)->GetFunction() );
	exports->Set( NanNew<String>("SetListenerOrientation"), NanNew<FunctionTemplate>(SetListenerOrientation)->GetFunction() );
}



/**
*	Initialize
*/
NODE_MODULE(openal, NodeInit)