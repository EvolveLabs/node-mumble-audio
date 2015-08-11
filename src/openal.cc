#include <node.h>
#include <nan.h>
#include <v8.h>
#include <delay_load_hook.h>

#include "ALCaptureDevice.h"
#include "ALDevice.h"

using namespace v8;
using namespace std;

// --------------------------------------------------
void NodeInit(Handle<Object> exports) {
	ALCaptureDevice::Init(exports);
	ALDevice::Init(exports);
}

/**
*	Initialize
*/
NODE_MODULE(openal, NodeInit)