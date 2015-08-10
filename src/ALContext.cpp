#include "ALContext.h"

using namespace v8;
using namespace node;

ALContext::ALContext() {
	this->context = alcGetCurrentContext();

	ALCdevice* device = alcGetContextsDevice(this->context);
	this->device = new ALDevice( device );
};

ALContext::~ALContext() {
	if(this->context != NULL) {
		delete this->device;
		alcDestroyContext(this->context);
	}
};

void ALContext::Init(Handle<Object> exports) {
	Local<FunctionTemplate> tpl = NanNew<FunctionTemplate>(New);
	tpl->SetClassName(NanNew<String>("Context"));
	tpl->InstanceTemplate()->SetInternalFieldCount(1);

	tpl->PrototypeTemplate()->Set(NanNew<String>("setDevice"), NanNew<FunctionTemplate>(SetDevice)->GetFunction());
	tpl->PrototypeTemplate()->Set(NanNew<String>("setSource"), NanNew<FunctionTemplate>(SetSource)->GetFunction());
	tpl->PrototypeTemplate()->Set(NanNew<String>("play"), NanNew<FunctionTemplate>(Play)->GetFunction());

	exports->Set(NanNew<String>("Context"), tpl->GetFunction());
};

NAN_METHOD(ALContext::SetDevice) {
	NanScope();

	if (args.Length() != 1) {
		NanThrowTypeError("Expected a Device as an argument.");
		NanReturnUndefined();
	}

	if ( !args[0]->IsObject() ) {
		NanThrowTypeError("Expected a Device as an argument.");
		NanReturnUndefined();
	}

	//todo...
	NanReturnUndefined();
};

NAN_METHOD(ALContext::SetSource) {
	NanScope();
	//todo...
	NanReturnUndefined();
}

NAN_METHOD(ALContext::Play) {
	NanScope();
	cout << "playing!" << endl;
	//todo...
	// alcMakeContextCurrent(this->context);

	NanReturnUndefined();
}

NAN_METHOD(ALContext::New) {
	NanScope();

	ALContext* ctx = new ALContext();
	ctx->Wrap(args.This());
	NanReturnValue(args.This());
};
