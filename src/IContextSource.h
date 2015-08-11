#pragma once

#include <node_object_wrap.h>

using namespace node;

class IContextSource : public ObjectWrap {
	protected:
		IContextSource() { }
		~IContextSource() { }

	public:
		virtual unsigned int SourceId() { return 0; }
		virtual void Start() { }
};
