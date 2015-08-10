#include "ALSource.h"

using namespace v8;
using namespace node;

// --------------------------------------------------------
ALSource::ALSource(ALWavData* data) {
    
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
ALSource::~ALSource() {
	alDeleteSources(1, &sourceid);
	alDeleteBuffers(1, &bufferid);
};

// --------------------------------------------------------
void ALSource::Init(Handle<Object> exports) {
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
NAN_METHOD(ALSource::New) {
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

	ALWavData* data = ObjectWrap::Unwrap<ALWavData>(args[0]->ToObject());
	ALSource* source = new ALSource(data);
	source->Wrap(args.This());
	NanReturnValue(args.This());
}

// --------------------------------------------------------
void ALSource::play() {
	alSourcePlay(sourceid);
}

// --------------------------------------------------------
void ALSource::setPosition(double x, double y, double z) {
	alSource3f(sourceid, AL_POSITION, (ALfloat)x, (ALfloat)y, (ALfloat)z);
}

// --------------------------------------------------------
void ALSource::setLoop(bool loop) {
	if(loop)
		alSourcei(sourceid, AL_LOOPING, AL_TRUE);
	else
		alSourcei(sourceid, AL_LOOPING, AL_FALSE);
}

// --------------------------------------------------------
NAN_METHOD(ALSource::Play) {
	NanScope();
	ALSource* obj = ObjectWrap::Unwrap<ALSource>(args.This());

	obj->play();
	
	NanReturnUndefined();
}

// --------------------------------------------------------
NAN_METHOD(ALSource::SetPosition) {
	NanScope();
	ALSource* obj = ObjectWrap::Unwrap<ALSource>(args.This());

	double x = args[0]->NumberValue();
	double y = args[1]->NumberValue();
	double z = args[2]->NumberValue();
	obj->setPosition(x, y, z);

	NanReturnUndefined();
}

// --------------------------------------------------------
NAN_METHOD(ALSource::SetLoop) {
	NanScope();
	ALSource* obj = ObjectWrap::Unwrap<ALSource>(args.This());

	bool loop = args[0]->BooleanValue();
	obj->setLoop( loop );

	NanReturnUndefined();
}


