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

#include "ALWavData.h"
#include "ALDevice.h"
#include "ALContext.h"
#include "ALSource.h"
#include "ALStream.h"

using namespace v8;
using namespace std;

// --------------------------------------------------
void NodeInit(Handle<Object> exports) {
	ALWavData::Init(exports);
	ALContext::Init(exports);
	ALDevice::Init(exports);
	ALSource::Init(exports);
	ALStream::Init(exports);
}

/**
*	Initialize
*/
NODE_MODULE(openal, NodeInit)