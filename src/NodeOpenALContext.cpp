#include "NodeOpenALContext.h"

using namespace v8;
using namespace node;

//vector<NodeOpenALContext*> NodeOpenALContext::contexts;


NodeOpenALContext::NodeOpenALContext(NodeOpenALDevice* dev) {
	context = alcCreateContext(dev->device, NULL);
    if(context==NULL) {
		std::cout << "cannot open context" << std::endl;
		return;
    }
};


NodeOpenALContext::~NodeOpenALContext() {
	if(context) {
		cout << "destroying context" << endl;
		alcDestroyContext(context);
	}
};

void NodeOpenALContext::Init(Handle<Object> exports) {
	// Prepare constructor template
	Local<FunctionTemplate> tpl = NanNew<FunctionTemplate>(New);
	tpl->SetClassName(NanNew<String>("Context"));
	tpl->InstanceTemplate()->SetInternalFieldCount(1);

	exports->Set(NanNew<String>("Context"), tpl->GetFunction());
}

NAN_METHOD(NodeOpenALContext::New) {
	NanScope();

	if (args.Length() != 1) {
		NanThrowTypeError("Expected a Device as an argument.");
		NanReturnUndefined();
	}

	if ( !args[0]->IsObject() ) {
		NanThrowTypeError("Expected a Device as an argument.");
		NanReturnUndefined();
	}

	NodeOpenALDevice* dev = ObjectWrap::Unwrap<NodeOpenALDevice>(args[0]->ToObject());
	
	NodeOpenALContext* ctx = new NodeOpenALContext( dev );
	//contexts.push_back( ctx );

	ctx->Wrap(args.This());
	NanReturnValue(args.This());
}

