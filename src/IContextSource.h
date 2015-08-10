#pragma once

class IContextSource {
	protected:
		IContextSource() { }
		~IContextSource() { }

	public:
		virtual unsigned int getAlcSourceId() { return 0; }
};
