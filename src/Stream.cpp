#include <node.h>
#include <node_buffer.h>
#include "Stream.h"

using namespace v8;
using namespace std;

// --------------------------------------------------------
void NodeOpenALStream::Init(Handle<Object> exports) {
	// Prepare constructor template
	Local<FunctionTemplate> tpl = NanNew<FunctionTemplate>(New);
	tpl->SetClassName(NanNew<String>("Stream"));
	tpl->InstanceTemplate()->SetInternalFieldCount(1);

	tpl->PrototypeTemplate()->Set(NanNew<String>("Ready"), NanNew<FunctionTemplate>(Ready)->GetFunction());
	tpl->PrototypeTemplate()->Set(NanNew<String>("Buffer"), NanNew<FunctionTemplate>(Buffer)->GetFunction());
	tpl->PrototypeTemplate()->Set(NanNew<String>("SetPosition"), NanNew<FunctionTemplate>(SetPosition)->GetFunction());
	tpl->PrototypeTemplate()->Set(NanNew<String>("GetPosition"), NanNew<FunctionTemplate>(GetPosition)->GetFunction());
	tpl->PrototypeTemplate()->Set(NanNew<String>("SetGain"), NanNew<FunctionTemplate>(SetGain)->GetFunction());

	exports->Set(NanNew<String>("Stream"), tpl->GetFunction());
}


// --------------------------------------------------------
NAN_METHOD(NodeOpenALStream::New) {
	NanScope();

	if (args.Length() != 3) {
		NanThrowTypeError("Expected 3 number arguments.");
		NanReturnUndefined();
	}

	if ( !args[0]->IsNumber() || !args[1]->IsNumber() || !args[2]->IsNumber()) {
		NanThrowTypeError("Expected 3 number arguments.");
		NanReturnUndefined();
	}

	double channels = args[0]->NumberValue();
	double bps = args[1]->NumberValue();
	double frequency = args[2]->NumberValue();

	NodeOpenALStream* stream = new NodeOpenALStream(channels, bps, frequency);
	stream->Wrap(args.This());
	NanReturnValue(args.This());
}


// --------------------------------------------------------
NAN_METHOD(NodeOpenALStream::Buffer) {
	NanScope();
	NodeOpenALStream* obj = ObjectWrap::Unwrap<NodeOpenALStream>(args.This());

	if (args.Length() != 1) {
		NanThrowTypeError("Expected a Buffer as an argument.");
		NanReturnUndefined();
	}

	Local<Value> buffer = args[0];
	size_t size = Buffer::Length( buffer->ToObject() );
	char* bufferdata = Buffer::Data( buffer->ToObject() );

	obj->buffer(size, bufferdata);
	NanReturnUndefined();
}


// --------------------------------------------------------
NAN_METHOD(NodeOpenALStream::Ready) {
	NanScope();
	NodeOpenALStream* obj = ObjectWrap::Unwrap<NodeOpenALStream>(args.This());
	NanReturnValue(NanNew<Boolean>( obj->ready() ));
}

// --------------------------------------------------------
NAN_METHOD(NodeOpenALStream::SetPosition) {
	NanScope();
	NodeOpenALStream* obj = ObjectWrap::Unwrap<NodeOpenALStream>(args.This());

	if (args.Length() != 3) {
		NanThrowTypeError("Expected 3 number arguments.");
		NanReturnUndefined();
	}

	if ( !args[0]->IsNumber() || !args[1]->IsNumber() || !args[2]->IsNumber()) {
		NanThrowTypeError("Expected 3 number arguments.");
		NanReturnUndefined();
	}

	double x = args[0]->NumberValue();
	double y = args[1]->NumberValue();
	double z = args[2]->NumberValue();
	obj->setPosition(x, y, z);

	NanReturnUndefined();
}

// --------------------------------------------------------
NAN_METHOD(NodeOpenALStream::GetPosition) {
	NanScope();
	NodeOpenALStream* obj = ObjectWrap::Unwrap<NodeOpenALStream>(args.This());
	
	ALfloat x;
	ALfloat y;
	ALfloat z;
	alGetSource3f(obj->sourceid, AL_POSITION, &x, &y, &z);

	Local<Object> position = NanNew<Object>();
	position->Set(NanNew<String>("x"), NanNew<Number>(x));
	position->Set(NanNew<String>("y"), NanNew<Number>(y));
	position->Set(NanNew<String>("z"), NanNew<Number>(z));

	NanReturnValue(position);
}


// --------------------------------------------------------
NAN_METHOD(NodeOpenALStream::SetGain) {
	NanScope();
	NodeOpenALStream* obj = ObjectWrap::Unwrap<NodeOpenALStream>(args.This());

	if (args.Length() != 1) {
		NanThrowTypeError("Expected a single number argument");
		NanReturnUndefined();
	}

	if ( !args[0]->IsNumber() ) {
		NanThrowTypeError("Expected a single number argument");
		NanReturnUndefined();
	}

	double x = args[0]->NumberValue();
	obj->setGain(x);

	NanReturnUndefined();
}

string ErrorCheck(ALenum error)
{
    if(error == AL_INVALID_NAME)
    {
        return "Invalid name";
    }
    else if(error == AL_INVALID_ENUM)
    {
        return " Invalid enum ";
    }
    else if(error == AL_INVALID_VALUE)
    {
        return " Invalid value ";
    }
    else if(error == AL_INVALID_OPERATION)
    {
        return " Invalid operation ";
    }
    else if(error == AL_OUT_OF_MEMORY)
    {
        return " Out of memory like! ";
    }
    return " Don't know ";
}

// -----------------------------------------------------
NodeOpenALStream::NodeOpenALStream(int channels, int bps, int _frequency) {
    if(channels==1) {
		if(bps==8) {
			format=AL_FORMAT_MONO8;
		} else {
        	format=AL_FORMAT_MONO16;               
		}
    } else {
		if(bps==8) {
			format=AL_FORMAT_STEREO8;
		} else {
			format=AL_FORMAT_STEREO16;             
		}      
    }

	/* Generate the buffers and sources */
	alGenBuffers(NUM_BUFFERS, buffers);
	alGenSources(1, &sourceid);
	ALenum error = alGetError();
	if(error != AL_NO_ERROR) {
		cout << "Error generating :( " << ErrorCheck(error) << endl;;
		return;
	}

	n = 0;
	frequency = _frequency;
	alSource3f(sourceid, AL_POSITION, 0, 0, 0);


	ALfloat source0Pos[]={ 2.0, 0.0, 2.0};	// Front left of the listener
	ALfloat source0Vel[]={ 0.0, 0.0, 0.0};

	alSourcef(sourceid, AL_PITCH, 1.0f);
	alSourcef(sourceid, AL_GAIN, 1.0f);
	alSourcefv(sourceid, AL_POSITION, source0Pos);
	alSourcefv(sourceid, AL_VELOCITY, source0Vel);
	alSourcei(sourceid, AL_LOOPING, AL_FALSE);
	alSourcef(sourceid, AL_ROLLOFF_FACTOR, 0.5f);

}

// -----------------------------------------------------
NodeOpenALStream::~NodeOpenALStream() {
	ALint val;
	/* Although mplayer is done giving us data, OpenAL may still be
	 * playing the remaining buffers. Wait until it stops. */
	do {
		alGetSourcei(sourceid, AL_SOURCE_STATE, &val);
	} while(val == AL_PLAYING);

	cout << "deleting source and buffers" << endl;
	alDeleteSources(1, &sourceid);
	alDeleteBuffers(NUM_BUFFERS, buffers);
}

// -----------------------------------------------------
void NodeOpenALStream::buffer(size_t size, char* data) {
	
	//cout << "received " << size << " bytes" << endl;
	ALenum error;

	// Prefill all of the buffers
	if(n < NUM_BUFFERS-1) {
		alBufferData(buffers[n], format, data, size, frequency);
		error = alGetError();
		if(error != AL_NO_ERROR) {
			cout << "Error loading :( " << ErrorCheck(error) << endl;
			return;
		}
		n++;

		if(n==NUM_BUFFERS-1) {
			alSourceQueueBuffers(sourceid, NUM_BUFFERS, buffers);
			alSourcePlay(sourceid);
		}
	} else {

		ALuint buffer;
		ALint val;

		/* Check if OpenAL is done with any of the queued buffers */
		alGetSourcei(sourceid, AL_BUFFERS_PROCESSED, &val);
		if(val <= 0)
			return;


		/* Pop the oldest queued buffer from the source, fill it
		 * with the new data, then requeue it */
		alSourceUnqueueBuffers(sourceid, 1, &buffer);
		alBufferData(buffer, format, data, size, frequency);
		alSourceQueueBuffers(sourceid, 1, &buffer);
		error = alGetError();
		if(error != AL_NO_ERROR) {
		    cout << "Error buffering :( " << ErrorCheck(error) << endl;
		    return;
		}

		/* Make sure the source is still playing, and restart it if needed. */
		alGetSourcei(sourceid, AL_SOURCE_STATE, &val);
		if(val != AL_PLAYING)
			alSourcePlay(sourceid);
	}
}

// -----------------------------------------------------
bool NodeOpenALStream::ready() {
	if(n < NUM_BUFFERS-1) return true;
	ALint val;
	alGetSourcei(sourceid, AL_BUFFERS_PROCESSED, &val);
	return (val > 0);
}

// -----------------------------------------------------
void NodeOpenALStream::setGain(float g) {
	alSourcef(sourceid, AL_GAIN, g);
}

// --------------------------------------------------------
void NodeOpenALStream::setPosition(double x, double y, double z) {
	cout << "SETTING: x=" << x << " y=" << y << " z=" << z << endl;
	alSource3f(sourceid, AL_POSITION, x, y, z);
}

