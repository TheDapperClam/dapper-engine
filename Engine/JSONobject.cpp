#include "JSONobject.h"

#include "Transform.h"
#include "Camera.h"
#include "Sprite.h"
#include "RectCollider.h"
#include "Label.h"

std::map<std::string, dap::FactoryMetaData*> dap::JSONobject::RegisteredFactories = {};

void dap::JSONobject::ClearFactories () {
	std::cout << "[JSONOBJECT] Clearing factories" << std::endl;
	for ( auto& pair : RegisteredFactories )
		delete RegisteredFactories[pair.first];
	RegisteredFactories.clear ();
}

dap::FactoryMetaData* const dap::JSONobject::GetMetaData ( const std::string typeID ) {
	if ( !RegisteredFactories.count ( typeID ) )
		return NULL;
	return RegisteredFactories[typeID];
}

const std::map<std::string, dap::FactoryMetaData*>* dap::JSONobject::GetRegisteredFactories () {
	return &RegisteredFactories;
}

void dap::JSONobject::RegisterEngineObjects () {
	RegisterFactory<dap::Transform> ( new dap::FactoryMetaData ( "Transform", true ) );
	RegisterFactory<dap::Camera> ( new dap::FactoryMetaData ( "Camera", true ) );
	RegisterFactory<dap::Sprite> ( new dap::FactoryMetaData ( "Sprite", true ) );
	RegisterFactory<dap::RectCollider> ( new dap::FactoryMetaData ( "RectCollider", true ) );
	RegisterFactory<dap::Label> ( new dap::FactoryMetaData ( "Label", true ) );
}
