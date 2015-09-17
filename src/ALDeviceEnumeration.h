#pragma once
#include <v8.h>
#include <node.h>
#include <nan.h>
#include <iostream>
#include <vector>
#include <queue>

#include <AL/al.h>
#include <AL/alc.h>

using namespace std;
using namespace Nan;
using namespace v8;

class ALDeviceEnumeration : public ObjectWrap {
	public:
		ALDeviceEnumeration();
		~ALDeviceEnumeration();

		static void Init(Handle<Object> exports);
		
		static NAN_METHOD(Devices);
};
