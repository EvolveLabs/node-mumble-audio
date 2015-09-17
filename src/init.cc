#include <delay_load_hook.h>

#include "ALCaptureDevice.h"
#include "ALPlaybackDevice.h"
#include "ALDeviceEnumeration.h"

using namespace v8;
using namespace Nan;
using namespace std;

// --------------------------------------------------
void NodeInit(Handle<Object> exports) {
	ALCaptureDevice::Init(exports);
	ALPlaybackDevice::Init(exports);
	ALDeviceEnumeration::Init(exports);
}

/**
*	Initialize
*/
NODE_MODULE(node_mumble_audio, NodeInit)
