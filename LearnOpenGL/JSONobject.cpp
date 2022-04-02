#include "JSONobject.h"

void dap::JSONobject::ClearFactories () {
	for ( auto &pair : GetRegisteredFactories () ) {
		std::string key = pair.first;
		delete GetRegisteredFactories ()[key];
		GetRegisteredFactories ()[key] = NULL;
	}
	GetRegisteredFactories ().clear ();
}
