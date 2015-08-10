#include "NodeOpenALSource.h"

using namespace v8;
using namespace node;

// --------------------------------------------------------
NodeOpenALSource::NodeOpenALSource(NodeWavData* data) {
    
    if(data->channel==1) {
		if(data->bps==8) {
			format=AL_FORMAT_MONO8;
		} else {
        	format=AL_FORMAT_MONO16;               
		}
    } else {
		if(data->bps==8) {
			format=AL_FORMAT_STEREO8;
		} else {
			format=AL_FORMAT_STEREO16;             
		}      
    }

    alGenBuffers(1, &bufferid);
    alBufferData(bufferid, format, data->data, data->size, data->samplerate);
    alGenSources(1, &sourceid);
	alSourcei(sourceid, AL_BUFFER, bufferid);
	alSource3f(sourceid, AL_POSITION, 0, 0, 0);
};

// --------------------------------------------------------
NodeOpenALSource::~NodeOpenALSource() {
	alDeleteSources(1, &sourceid);
	alDeleteBuffers(1, &bufferid);
};

// --------------------------------------------------------
void NodeOpenALSource::Init(Handle<Object> exports) {
  // Prepare constructor template
  Local<FunctionTemplate> tpl = NanNew<FunctionTemplate>(New);
  tpl->SetClassName(NanNew<String>("Source"));
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  tpl->PrototypeTemplate()->Set(NanNew<String>("Play"), NanNew<FunctionTemplate>(Play)->GetFunction());
  tpl->PrototypeTemplate()->Set(NanNew<String>("SetPosition"), NanNew<FunctionTemplate>(SetPosition)->GetFunction());
  tpl->PrototypeTemplate()->Set(NanNew<String>("SetLoop"), NanNew<FunctionTemplate>(SetLoop)->GetFunction());

  exports->Set(NanNew<String>("Source"), tpl->GetFunction());
}

// --------------------------------------------------------
NAN_METHOD(NodeOpenALSource::New) {
	NanScope();

	if (!args.IsConstructCall()) {
		NanThrowTypeError("Use the new operator to construct the Source.");
		NanReturnUndefined();
	}

	if (args.Length() != 1) {
		NanThrowTypeError("Expected a single argument.");
		NanReturnUndefined();
	}

	if ( !args[0]->IsObject() ) {
		NanThrowTypeError("Wrong arguments: expected object.");
		NanReturnUndefined();
	}

	NodeWavData* data = ObjectWrap::Unwrap<NodeWavData>(args[0]->ToObject());
	NodeOpenALSource* source = new NodeOpenALSource(data);
	source->Wrap(args.This());
	NanReturnValue(args.This());
}

// --------------------------------------------------------
void NodeOpenALSource::play() {
	alSourcePlay(sourceid);
}

// --------------------------------------------------------
void NodeOpenALSource::setPosition(double x, double y, double z) {
	alSource3f(sourceid, AL_POSITION, x, y, z);
}

// --------------------------------------------------------
void NodeOpenALSource::setLoop(bool loop) {
	if(loop)
		alSourcei(sourceid, AL_LOOPING, AL_TRUE);
	else
		alSourcei(sourceid, AL_LOOPING, AL_FALSE);
}

// --------------------------------------------------------
NAN_METHOD(NodeOpenALSource::Play) {
	NanScope();
	NodeOpenALSource* obj = ObjectWrap::Unwrap<NodeOpenALSource>(args.This());

	obj->play();
	
	NanReturnUndefined();
}

// --------------------------------------------------------
NAN_METHOD(NodeOpenALSource::SetPosition) {
	NanScope();
	NodeOpenALSource* obj = ObjectWrap::Unwrap<NodeOpenALSource>(args.This());

	double x = args[0]->NumberValue();
	double y = args[1]->NumberValue();
	double z = args[2]->NumberValue();
	obj->setPosition(x, y, z);

	NanReturnUndefined();
}

// --------------------------------------------------------
NAN_METHOD(NodeOpenALSource::SetLoop) {
	NanScope();
	NodeOpenALSource* obj = ObjectWrap::Unwrap<NodeOpenALSource>(args.This());

	bool loop = args[0]->BooleanValue();
	obj->setLoop( loop );

	NanReturnUndefined();
}


