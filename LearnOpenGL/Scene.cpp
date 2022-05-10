#include "Scene.h"

#include <fstream>
#include <iostream>
#include "nlohmann/json.hpp"

std::map<const std::string, dap::Scene* const> dap::Scene::loadedScenes = {};

dap::Scene::Scene ( const std::string fileName ) {
	std::ifstream ifs ( fileName );
	if ( ifs.good () ) {
		nlohmann::json json = nlohmann::json::parse ( ifs );
		name = json["name"];
		for ( auto obj : json["gameobjects"] ) {
			dap::GameObject* newObj = static_cast<dap::GameObject*>( dap::JSONobject::Factory ( obj["type"] ) );
			allGameObjects.push_back ( newObj );
			newObj->Deserialize ( &obj );
		}
		loadedScenes.insert ( std::pair<const std::string, dap::Scene* const> ( name, this ) );
	}
	ifs.close ();
}

dap::Scene::~Scene () {
	for ( int i = 0; i < allGameObjects.size (); i++ ) {
		delete allGameObjects.at ( i );
		allGameObjects[i] = NULL;
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

const dap::Scene* dap::Scene::GetScene ( const std::string id ) {
	if ( !loadedScenes.count ( id ) )
		return NULL;
	return loadedScenes[id];
}

void dap::Scene::Save ( const dap::Scene* scene, const std::string fileName ) {
	std::ofstream ofs ( fileName );
	if ( ofs.good () ) {
		std::vector<nlohmann::json> serializedGameObjects;
		for ( const auto obj : scene->allGameObjects ) {
			serializedGameObjects.push_back ( obj->Serialize () );
		}

		nlohmann::json json {
			{ "name", scene->name },
			{ "gameobjects", serializedGameObjects }
		};
		ofs << json;
	}
	ofs.close ();
}
