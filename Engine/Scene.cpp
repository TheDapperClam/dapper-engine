#include "Scene.h"

#include "Editor.h"
#include <fstream>
#include "GameObject.h"
#include <iostream>
#include <stdexcept>
#include "nlohmann/json.hpp"

dap::Scene* dap::Scene::CurrentScene = NULL;

dap::Scene::Scene ( const std::string fileName ) {
	std::ifstream ifs ( fileName );
	if ( ifs.good () ) {
		nlohmann::json json = nlohmann::json::parse ( ifs );
		name = json["name"];
		path = fileName;
		std::cout << "[SCENE] Loading scene: " << name << std::endl;
		for ( auto obj : json["gameobjects"] ) {
			dap::GameObject* newObj = new dap::GameObject ( this );
			newObj->Deserialize ( &obj );
			allGameObjects.insert ( std::pair<std::string, dap::GameObject*> ( newObj->GetName (), newObj ) );
		}
		std::cout << "[SCENE] Loading complete" << std::endl;
	}
	ifs.close ();
}

dap::Scene::~Scene () {
	std::cout << "[SCENE] Unloading scene: " << name << std::endl;
	// Clear the editor's selected objects when unloading the scene, otherwise we crash when trying to access unloaded objects
	if ( dap::Editor::GetCurrent () )
		dap::Editor::GetCurrent ()->ClearSelected ();
	for ( auto& pair : allGameObjects ) {
		delete pair.second;
	}
	allGameObjects.clear ();
	if ( CurrentScene == this )
		CurrentScene = NULL;
}

void dap::Scene::DeleteGameObject ( dap::GameObject* object ) {
	if ( !object )
		return;
	allGameObjects.erase ( object->GetName () );
	delete object;
}

std::map<std::string, dap::GameObject*>* const dap::Scene::GetAllGameObjects () {
	return &allGameObjects;
}

dap::Scene* const dap::Scene::GetCurrent () {
	return CurrentScene;
}

dap::GameObject* dap::Scene::GetGameObject ( std::string name ) {
	return allGameObjects.contains ( name ) ? allGameObjects[name] : NULL;
}

std::string dap::Scene::GetName () {
	return name;
}

std::string dap::Scene::GetPath () {
	return path;
}

dap::Scene* const dap::Scene::Load ( const std::string fileName ) {
	UnloadCurrent ();
	CurrentScene = new dap::Scene ( fileName );
	return CurrentScene;
}

void dap::Scene::Save ( const std::string fileName ) {
	std::ofstream ofs ( fileName );
	if ( ofs.good () ) {
		std::cout << "[SCENE] Saving: " << fileName << std::endl;
		std::vector<nlohmann::json> serializedGameObjects;
		for ( const auto& pair : allGameObjects ) {
			serializedGameObjects.push_back ( pair.second->Serialize () );
		}

		nlohmann::json json {
			{ "name", name },
			{ "gameobjects", serializedGameObjects }
		};
		ofs << json;
		std::cout << "[SCENE] Save complete" << std::endl;
	}
	ofs.close ();
}

dap::GameObject* dap::Scene::SpawnGameObject ( std::string name ) {
	if ( allGameObjects.contains ( name ) )
		throw std::logic_error ( "[SCENE] GameObject with name " + name + " already exists in scene " + this->name );
	dap::GameObject* newObj = new dap::GameObject ( this );
	newObj->SetName ( name );
	allGameObjects.insert ( std::pair<std::string, dap::GameObject*> ( name, newObj ) );
	return newObj;
}

void dap::Scene::UnloadCurrent () {
	if ( !CurrentScene )
		return;
	delete CurrentScene;
	CurrentScene = NULL;
}
