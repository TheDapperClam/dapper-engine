#include "GameObject.h"

#include <filesystem>
#include <fstream>
#include "imgui.h"
#include <iostream>
#include "Scene.h"
#include <stdexcept>
#include "Transform.h"
#include <vector>

unsigned long dap::GameObject::NextId = 0;

dap::GameObject::GameObject ( dap::Scene* owningScene, std::string startingName ) : ID(NextId) {
	if ( !owningScene )
		throw std::invalid_argument ( "All GameObjects must be assigned an owning scene" );
	NextId++;
	name = startingName;
	transform = AddComponent<Transform> ();
	this->owningScene = owningScene;
}

dap::GameObject::~GameObject () {
	if ( owningScene )
		owningScene->RemoveGameObject ( this, false );
	for ( const auto& COMP_PAIR : components ) {
		for ( int i = COMP_PAIR.second.size () - 1; i >= 0; i-- ) {
			delete COMP_PAIR.second[i];
		}
	}
}

void dap::GameObject::AddComponent ( dap::Component* const comp ) {
	if ( !comp )
		return;
	const std::type_index KEY = typeid( *comp );
	if ( !components.count ( KEY ) )
		components.insert ( std::pair < std::type_index, std::vector<dap::Component*>> ( KEY, {} ) );
	components[KEY].push_back ( comp );
	comp->SetOwner ( this );
	comp->Initialize ();
}

void dap::GameObject::Destroy () {
	shouldBeDestroyed = true;
}

const std::vector<dap::Component*> dap::GameObject::GetAllComponentsList () {
	std::vector<dap::Component*> allComponents;
	for ( const auto& COMP_PAIR : components ) {
		allComponents.insert ( allComponents.end (), COMP_PAIR.second.begin (), COMP_PAIR.second.end () );
	}
	return allComponents;
}

std::string dap::GameObject::GetName () {
	return name;
}

unsigned long dap::GameObject::GetId () {
	return ID;
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

void dap::GameObject::RemoveComponent ( dap::Component* component, bool shouldDelete ) {
	if ( !component )
		return;
	const std::type_index KEY = typeid( *component );
	if ( !components.count ( KEY ) )
		return;
	components[KEY].erase ( std::remove ( components[KEY].begin (), components[KEY].end (), component ), components[KEY].end () );
	if ( shouldDelete )
		delete component;
}

void dap::GameObject::SavePrefab ( std::string fileName ) {
	std::filesystem::create_directory ( std::filesystem::path ( fileName ).parent_path () );

	std::ofstream ofs ( fileName );
	if ( ofs.good () ) {
		std::cout << "[GAMEOBJECT] Saving prefab: " << fileName << std::endl;
		ofs << Serialize ();
		std::cout << "[GAMEOBJECT] Prefab saved" << std::endl;
	}
	ofs.close ();
}

void dap::GameObject::SetName ( std::string newName ) {
	name = newName;
}

bool dap::GameObject::ShouldBeDestroyed () const {
	return shouldBeDestroyed;
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
	nlohmann::json json = dap::JSONObject::Serialize ();
	json["name"] = name;
	json["components"] = jsonComponents;
	//JSON_APPEND_TYPE ( json )
	return json;
}
