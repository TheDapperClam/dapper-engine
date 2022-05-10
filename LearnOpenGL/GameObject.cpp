#include "GameObject.h"

JSON_REGISTER ( dap::GameObject );

unsigned int dap::GameObject::nextId = 0;

dap::GameObject::GameObject ( std::string startingName ) : id(nextId) {
	nextId++;
	name = startingName;
	transform = AddComponent<Transform> ();
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

const std::string* dap::GameObject::GetName () const {
	return &name;
}

const unsigned int dap::GameObject::GetId () const {
	return id;
}

dap::Transform* const dap::GameObject::GetTransform () const {
	return transform;
}

void dap::GameObject::SetName ( const std::string newName ) {
	name = newName;
}

void dap::GameObject::Deserialize ( const nlohmann::json* json ) {
	SetName ( json->at ( "name" ) );

	for ( auto comp : json->at ( "components" ) ) {
		if ( comp["type"] == typeid( dap::Transform ).name () ) {
			transform->Deserialize ( &comp );
			continue;
		}
		dap::Component* const newComponent = static_cast<dap::Component*>( Factory ( comp["type"] ) );
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
