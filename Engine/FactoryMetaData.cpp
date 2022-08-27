#include "FactoryMetaData.h"

dap::FactoryMetaData::FactoryMetaData () {

}

dap::FactoryMetaData::FactoryMetaData ( std::string name, bool isComponent ) {
	Name = name;
	IsComponent = isComponent;
}

dap::FactoryMetaData::~FactoryMetaData () {
	if ( factory )
		delete factory;
}
