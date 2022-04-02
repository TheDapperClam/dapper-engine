#include "Scene.h"

std::map<const std::string, dap::Scene* const> dap::Scene::loadedScenes = {};

dap::Scene::Scene ( const std::string fileName, const std::string id ) : name ( id ) {
	std::ifstream ifs ( fileName );
	if ( ifs.good () ) {
		nlohmann::json json = nlohmann::json::parse ( ifs );
		for ( auto obj : json["gameobjects"] ) {
			dap::GameObject* newObj = static_cast<dap::GameObject*>( dap::JSONobject::Factory ( obj["type"] ) );
			allGameObjects.push_back ( newObj );
			newObj->Deserialize ( &obj );
		}
		loadedScenes.insert ( std::pair<const std::string, dap::Scene* const> ( id, this ) );
	}
	ifs.close ();
}

dap::Scene::~Scene () {
	for ( int i = 0; i < allGameObjects.size (); i++ ) {
		delete allGameObjects.at ( i );
		allGameObjects[i] = NULL;
	}
	allGameObjects.clear ();
	loadedScenes.erase ( name );
}

void dap::Scene::ClearAll () {
	for ( const auto pair : loadedScenes ) {
		const std::string key = pair.first;
		delete loadedScenes[key];
	}
	loadedScenes.clear ();
}

const dap::Scene* dap::Scene::GetScene ( const std::string id ) {
	if ( !loadedScenes.count ( id ) )
		return NULL;
	return loadedScenes[id];
}
