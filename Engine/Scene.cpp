#include "Scene.h"

#include "Editor.h"
#include <filesystem>
#include <fstream>
#include "GameObject.h"
#include <iostream>
#include <stdexcept>

dap::Scene* dap::Scene::CurrentScene = NULL;
std::unordered_map<std::string, nlohmann::json> dap::Scene::PrefabCache = {};
std::string dap::Scene::SceneToLateLoad = "";
bool dap::Scene::WantsToLateLoad = false;

dap::Scene::Scene ( const std::string fileName ) {
	path = fileName;

	if ( !std::filesystem::exists ( fileName ) ) {
		const std::string FILE_NAME_WITH_EXTENSION = std::filesystem::path ( fileName ).filename ().string ();
		const int EXTENSION_START = FILE_NAME_WITH_EXTENSION.find ( '.' );
		const std::string FILE_NAME_WITHOUT_EXTENSION = FILE_NAME_WITH_EXTENSION.substr ( 0, EXTENSION_START );
		name = FILE_NAME_WITHOUT_EXTENSION;
		return;
	}

	std::ifstream ifs ( fileName );
	if ( ifs.good () ) {
		nlohmann::json json = nlohmann::json::parse ( ifs );
		name = json["name"];
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
	const auto GAMEOBJECTS_TO_DELETE = allGameObjects;
	for ( const auto& GAMEOBJECT_PAIR : GAMEOBJECTS_TO_DELETE ) {
		delete allGameObjects[GAMEOBJECT_PAIR.first];
	}
	allGameObjects.clear ();
	if ( CurrentScene == this )
		CurrentScene = NULL;
}

void dap::Scene::BeginLateLoad () {
	if ( !WantsToLateLoad )
		return;
	WantsToLateLoad = false;
	LoadInstant ( SceneToLateLoad );
}

nlohmann::json dap::Scene::CachePrefab ( std::string fileName ) {
	nlohmann::json json;
	if ( std::filesystem::exists ( fileName ) ) {
		std::ifstream ifs ( fileName );
		if ( ifs.good () ) {
			json = nlohmann::json::parse ( ifs );
			if ( !PrefabCache.count ( fileName ) )
				PrefabCache.insert ( std::pair<std::string, nlohmann::json> ( fileName, json ) );
			else
				PrefabCache[fileName] = json;
		}
		ifs.close ();
	}
	return json;
}

void dap::Scene::ClearPrefabCache () {
	PrefabCache.clear ();
}

void dap::Scene::DeleteMarkedObjects () {
	for ( const auto& OBJECT : objectsToDelete )
		delete OBJECT;
	objectsToDelete.clear ();
}

const std::vector<dap::GameObject*> dap::Scene::GetAllGameObjectsList () {
	std::vector<dap::GameObject*> list;
	for ( const auto& GAMEOBJECT_PAIR : allGameObjects )
		list.push_back ( GAMEOBJECT_PAIR.second );
	return list;
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

void dap::Scene::Load ( const std::string fileName ) {
	WantsToLateLoad = true;
	SceneToLateLoad = fileName;
}

dap::Scene* const dap::Scene::LoadInstant ( const std::string fileName ) {
	UnloadCurrent ();
	CurrentScene = new dap::Scene ( fileName );
	if ( dap::Editor::GetCurrent () )
		dap::Editor::GetCurrent ()->RefreshSelected ();
	return CurrentScene;
}

void dap::Scene::MarkObjectForLateDeletion ( dap::GameObject* object ) {
	object->markedForDeletion = true;
	objectsToDelete.push_back ( object );
}

void dap::Scene::RemoveGameObject ( dap::GameObject* object, bool shouldDelete ) {
	if ( !object || !allGameObjects.count ( object->GetName () ) )
		return;
	allGameObjects.erase ( object->GetName () );
	if ( shouldDelete )
		delete object;
}

void dap::Scene::Save ( const std::string fileName ) {
	std::filesystem::create_directory ( std::filesystem::path ( fileName ).parent_path () );

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

dap::GameObject* const dap::Scene::SpawnGameObject ( std::string name ) {
	if ( allGameObjects.contains ( name ) )
		throw std::logic_error ( "[SCENE] GameObject with name " + name + " already exists in scene " + this->name );
	dap::GameObject* newObj = new dap::GameObject ( this );
	newObj->SetName ( name );
	allGameObjects.insert ( std::pair<std::string, dap::GameObject*> ( name, newObj ) );
	return newObj;
}

dap::GameObject* const dap::Scene::SpawnPrefab ( std::string fileName, bool ignoreCache ) {
	dap::GameObject* newPrefab = NULL;
	nlohmann::json json = !ignoreCache && PrefabCache.count ( fileName ) ? PrefabCache[fileName] : CachePrefab ( fileName );
	if ( !json.is_null () ) {
		newPrefab = new dap::GameObject ( this );
		newPrefab->Deserialize ( &json );
		if ( GetGameObject ( newPrefab->GetName () ) )
			newPrefab->SetName ( newPrefab->GetName () + "_" + std::to_string ( newPrefab->GetId () ) );
		allGameObjects.insert ( std::pair<std::string, dap::GameObject*> ( newPrefab->GetName (), newPrefab ) );
	}
	return newPrefab;
}

void dap::Scene::UnloadCurrent () {
	if ( !CurrentScene )
		return;
	delete CurrentScene;
	CurrentScene = NULL;
	dap::GameObject::ResetNextId ();
}
