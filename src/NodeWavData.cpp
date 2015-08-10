#include <node.h>
#include "NodeWavData.h"

bool isBigEndian() {
	int a=1;
	return !((char*)&a)[0];
}
 
int convertToInt(char* buffer,int len) {
	int a=0;
	if(!isBigEndian())
		for(int i=0;i<len;i++)
			((char*)&a)[i]=buffer[i];
	else
		for(int i=0;i<len;i++)
			((char*)&a)[3-i]=buffer[i];    
	return a;
}

char* loadWAV(const char* fn, int& chan, int& samplerate, int& bps, int& size)
{
	char buffer[4];
	std::ifstream in(fn, std::ios::binary);
	in.read(buffer,4);
	if(strncmp(buffer, "RIFF", 4)!=0) {
		std::cout << fn << " is not a valid WAVE file"  << std::endl;
		return NULL;
	}
	in.read(buffer,4);
	in.read(buffer,4);      //WAVE
	in.read(buffer,4);      //fmt
	in.read(buffer,4);      //16
	in.read(buffer,2);      //1
	in.read(buffer,2);
	chan=convertToInt(buffer,2);
	in.read(buffer,4);
	samplerate=convertToInt(buffer,4);
	in.read(buffer,4);
	in.read(buffer,2);
	in.read(buffer,2);
	bps=convertToInt(buffer,2);
	in.read(buffer,4);      //data
	in.read(buffer,4);
	size=convertToInt(buffer,4);
	char* data=new char[size];
	in.read(data,size);
	return data;   
}

// ---------------------------------------------------
NodeWavData::NodeWavData(const char* path) {
	data = loadWAV(path, channel, samplerate, bps, size);
};

// ---------------------------------------------------
NodeWavData::~NodeWavData() {
	delete[] data;
};

// ---------------------------------------------------
void NodeWavData::Init(Handle<Object> exports) {
	// Prepare constructor template
	Local<FunctionTemplate> tpl = NanNew<FunctionTemplate>( New );
	tpl->SetClassName( NanNew<String>("WavData") );
	tpl->InstanceTemplate()->SetInternalFieldCount( 1 );
	
	exports->Set(NanNew<String>("WavData"), tpl->GetFunction());
}

// ---------------------------------------------------
NAN_METHOD(NodeWavData::New) {
	NanScope();
	if (args.Length() == 0 || !args[0]->IsString()) {
		NanThrowTypeError("Expected a path string argument.");
		NanReturnUndefined();
	}

	char* path = *NanAsciiString(args[0]);

	ifstream my_file(path);
	if(!my_file.good()) {
		NanThrowError("File not found");
		NanReturnUndefined();
	}

	NodeWavData* wav = new NodeWavData( path );
	wav->Wrap( args.This() );
	NanReturnValue(args.This());
}