#include "Scene.h"

#include <fstream>
#include <stdexcept>
#include "nlohmann/json.hpp"

std::map<const std::string, dap::Scene* const> dap::Scene::loadedScenes = {};

dap::Scene::Scene ( const std::string fileName ) {
	std::ifstream ifs ( fileName );
	if ( ifs.good () ) {
		nlohmann::json json = nlohmann::json::parse ( ifs );
		name = json["name"];
		for ( auto obj : json["gameobjects"] ) {
			dap::GameObject* newObj = new dap::GameObject ( this );
			newObj->Deserialize ( &obj );
			allGameObjects.insert ( std::pair<std::string, dap::GameObject*> ( newObj->GetName (), newObj ) );
		}
		loadedScenes.insert ( std::pair<const std::string, dap::Scene* const> ( name, this ) );
	}
	ifs.close ();
}

dap::Scene::~Scene () {
	for ( auto& pair : allGameObjects ) {
		delete pair.second;
	}
	allGameObjects.clear ();
	if ( eraseOnDelete )
		loadedScenes.erase ( name );
}

void dap::Scene::ClearAll () {
	for ( const auto pair : loadedScenes ) {
		const std::string key = pair.first;
		loadedScenes.at ( key )->eraseOnDelete = false;
		delete loadedScenes.at ( key );
	}
	loadedScenes.clear ();
}

void dap::Scene::DeleteGameObject ( dap::GameObject* object ) {
	if ( !object )
		return;
	allGameObjects.erase ( object->GetName () );
	delete object;
}

dap::GameObject* dap::Scene::GetGameObject ( std::string name ) {
	return allGameObjects.contains ( name ) ? allGameObjects[name] : NULL;
}

std::string dap::Scene::GetName () {
	return name;
}

dap::Scene* dap::Scene::GetScene ( const std::string id ) {
	if ( !loadedScenes.count ( id ) )
		return NULL;
	return loadedScenes[id];
}

void dap::Scene::Save ( const dap::Scene* scene, const std::string fileName ) {
	std::ofstream ofs ( fileName );
	if ( ofs.good () ) {
		std::vector<nlohmann::json> serializedGameObjects;
		for ( const auto& pair : scene->allGameObjects ) {
			serializedGameObjects.push_back ( pair.second->Serialize () );
		}

		nlohmann::json json {
			{ "name", scene->name },
			{ "gameobjects", serializedGameObjects }
		};
		ofs << json;
	}
	ofs.close ();
}

dap::GameObject* dap::Scene::SpawnGameObject ( std::string name ) {
	if ( allGameObjects.contains ( name ) )
		throw std::logic_error ( "GameObject with name " + name + " already exists in scene " + this->name );
	dap::GameObject* newObj = new dap::GameObject ( this );
	newObj->SetName ( name );
	allGameObjects.insert ( std::pair<std::string, dap::GameObject*> ( name, newObj ) );
	return newObj;
}
