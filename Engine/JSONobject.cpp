#include "JSONObject.h"

std::map<std::string, dap::FactoryMetaData*> dap::JSONObject::RegisteredFactories = {};

void dap::JSONObject::ClearFactories () {
	std::cout << "[JSONOBJECT] Clearing factories" << std::endl;
	for ( auto& pair : RegisteredFactories )
		delete RegisteredFactories[pair.first];
	RegisteredFactories.clear ();
}

dap::FactoryMetaData* const dap::JSONObject::GetMetaData ( const std::string typeID ) {
	if ( !RegisteredFactories.count ( typeID ) )
		return NULL;
	return RegisteredFactories[typeID];
}

const std::map<std::string, dap::FactoryMetaData*>* dap::JSONObject::GetRegisteredFactories () {
	return &RegisteredFactories;
}

nlohmann::json dap::JSONObject::Serialize () {
	nlohmann::json json;
	json["type"] = typeid( *this ).name ();
	return json;
}
