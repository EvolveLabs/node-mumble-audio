#include <node.h>
#include "NodeOpenAlDevice.h"

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
