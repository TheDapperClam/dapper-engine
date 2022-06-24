#include "GameObject.h"
#include "Scene.h"
#include <stdexcept>
#include <iostream>

unsigned int dap::GameObject::nextId = 0;

dap::GameObject::GameObject ( dap::Scene* owningScene, std::string startingName ) : id(nextId) {
	if ( !owningScene )
		throw std::invalid_argument ( "All GameObjects must be assigned an owning scene" );
	nextId++;
	name = startingName;
	transform = AddComponent<Transform> ();
	this->owningScene = owningScene;
}

dap::GameObject::~GameObject () {
	for ( auto const &p : components ) {
		for ( auto comp : p.second ) {
			delete comp;
			comp = NULL;
		}
	}
	components.clear ();
}

void dap::GameObject::AddComponent ( dap::Component* const comp ) {
	std::type_index key = typeid( *comp );
	if ( !components.count ( key ) )
		components.insert ( std::pair < std::type_index, std::vector<dap::Component*>> ( key, {} ) );
	components[key].push_back ( comp );
	comp->SetOwner ( this );
	comp->Initialize ();
}

void dap::GameObject::Destroy () {
	this->owningScene->DeleteGameObject ( this );
}

std::string dap::GameObject::GetName () {
	return name;
}

unsigned int dap::GameObject::GetId () {
	return id;
}

dap::Transform* const dap::GameObject::GetTransform () {
	return transform;
}

void dap::GameObject::SetName ( std::string newName ) {
	name = newName;
}

void dap::GameObject::Deserialize ( const nlohmann::json* json ) {
	SetName ( json->at ( "name" ) );

	for ( const auto& comp : json->at ( "components" ) ) {
		if ( comp["type"] == typeid( dap::Transform ).name () ) {
			transform->Deserialize ( &comp );
			continue;
		}
		const std::string compType = comp["type"];
		dap::Component* const newComponent = static_cast<dap::Component*>( Factory ( compType ) );
		if ( !newComponent ) {
			std::cout << "Failed to deserialize component " + compType + " on object " + GetName () << std::endl;
			continue;
		}
		newComponent->Deserialize ( &comp );
		AddComponent ( newComponent );
	}
}

const nlohmann::json dap::GameObject::Serialize () {
	std::vector<nlohmann::json> jsonComponents;
	for ( auto pair : components ) {
		std::type_index key = (std::type_index)pair.first;
		for ( auto *component : components[key] ) {
			jsonComponents.push_back ( component->Serialize () );
		}
	}
	nlohmann::json json {
		{ "type", typeid(*this).name () },
		{ "name", name },
		{ "components", jsonComponents },
	};
	return json;
}
