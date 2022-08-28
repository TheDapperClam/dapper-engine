#include "GameObject.h"
#include "Scene.h"
#include <stdexcept>
#include "imgui.h"
#include <iostream>
#include "Transform.h"
#include <vector>

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
		}
	}
	components.clear ();
}

void dap::GameObject::AddComponent ( dap::Component* const comp ) {
	if ( !comp )
		return;
	const std::type_index KEY = typeid( *comp );
	std::cout << "[GAMEOBJECT] Adding component: " << KEY.name () << " to " << name << std::endl;
	if ( !components.count ( KEY ) )
		components.insert ( std::pair < std::type_index, std::vector<dap::Component*>> ( KEY, {} ) );
	components[KEY].push_back ( comp );
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

void dap::GameObject::ImGuiWidgetsBegin () {
	bool firstComp = true;
	for ( const auto& compPair : components ) {
		ImGui::PushID ( this );
		for ( int i = compPair.second.size () - 1; i >= 0; i-- ) {
			dap::Component* comp = compPair.second.at ( i );
			ImGui::PushID ( comp );
			if ( !firstComp )
				ImGui::Separator ();
			firstComp = false;
			if ( ImGui::Button ( "X" ) ) {
				RemoveComponent ( comp );
				ImGui::PopID ();
				continue;
			}
			ImGui::SameLine ();
			comp->ImGuiWidgetsBegin ();
			ImGui::PopID ();
		}
		ImGui::PopID ();
	}
}

void dap::GameObject::RemoveComponent ( dap::Component* component ) {
	if ( !component )
		return;
	const std::type_index KEY = typeid( *component );
	if ( !components.count ( KEY ) )
		return;
	std::cout << "[GAMEOBJECT] Removing component: " << KEY.name () << " from " << name << std::endl;
	components[KEY].erase ( std::remove ( components[KEY].begin (), components[KEY].end (), component ), components[KEY].end () );
	delete component;
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
			std::cout << "[GAMEOBJECT] Failed to deserialize component " + compType + " on object " + GetName () << std::endl;
			continue;
		}
		newComponent->Deserialize ( &comp );
		AddComponent ( newComponent );
	}
}

nlohmann::json dap::GameObject::Serialize () {
	std::vector<nlohmann::json> jsonComponents;
	for ( auto pair : components ) {
		std::type_index key = (std::type_index)pair.first;
		for ( auto *component : components[key] ) {
			jsonComponents.push_back ( component->Serialize () );
		}
	}
	nlohmann::json json {
		{ "name", name },
		{ "components", jsonComponents },
	};
	JSON_APPEND_TYPE ( json )
	return json;
}
