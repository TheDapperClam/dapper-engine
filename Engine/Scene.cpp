#include "Scene.h"

#include "Editor.h"
#include <filesystem>
#include <fstream>
#include "GameObject.h"
#include <iostream>
#include <stdexcept>

std::vector<dap::GameObject*> dap::Scene::AllGameObjects = {};
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
			std::cout << "[SCENE] Loaded GameObject: " << newObj->GetName () << std::endl;
			localGameObjects.insert ( std::pair<std::string, dap::GameObject*> ( newObj->GetName (), newObj ) );
			AllGameObjects.push_back ( newObj );
		}
		std::cout << "[SCENE] Loading complete" << std::endl;
	}
	ifs.close ();
}

dap::Scene::~Scene () {
	std::cout << "[SCENE] Unloading scene: " << name << std::endl;
	const auto GAMEOBJECTS_TO_DELETE = localGameObjects;
	for ( const auto& GAMEOBJECT_PAIR : GAMEOBJECTS_TO_DELETE ) {
		delete localGameObjects[GAMEOBJECT_PAIR.first];
	}
	localGameObjects.clear ();
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

const std::vector<dap::GameObject*>* dap::Scene::GetAllGameObjects () {
	return &AllGameObjects;
}

dap::Scene* const dap::Scene::GetCurrent () {
	return CurrentScene;
}

dap::GameObject* dap::Scene::GetGameObject ( std::string name ) {
	return localGameObjects.contains ( name ) ? localGameObjects[name] : NULL;
}

const std::unordered_map<std::string, dap::GameObject*>* dap::Scene::GetLocalGameObjects () {
	return &localGameObjects;
}

std::string dap::Scene::GetName () {
	return name;
}

std::string dap::Scene::GetPath () {
	return path;
}

void dap::Scene::LoadLate ( const std::string fileName ) {
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
	if ( !object || !localGameObjects.count ( object->GetName () ) )
		return;
	localGameObjects.erase ( object->GetName () );
	AllGameObjects.erase ( std::remove ( AllGameObjects.begin (), AllGameObjects.end (), object ), AllGameObjects.end () );
	if ( shouldDelete )
		delete object;
}

void dap::Scene::Save ( const std::string fileName ) {
	std::filesystem::path fileNamePath ( fileName );
	const std::string TEMP_FILE = fileNamePath.replace_filename ( "temp_" + fileNamePath.filename ().string () ).string ();
	std::filesystem::create_directory ( std::filesystem::path ( fileName ).parent_path () );

	std::ofstream ofs ( TEMP_FILE );
	if ( ofs.good () ) {
		std::cout << "[SCENE] Saving: " << fileName << std::endl;
		std::vector<nlohmann::json> serializedGameObjects;
		for ( const auto& pair : localGameObjects ) {
			serializedGameObjects.push_back ( pair.second->Serialize () );
		}

		nlohmann::json json {
			{ "name", name },
			{ "gameobjects", serializedGameObjects }
		};
		ofs << json;
		ofs.close ();
		std::filesystem::rename ( TEMP_FILE, fileName );
		std::cout << "[SCENE] Save complete" << std::endl;
	} else {
		ofs.close ();
		std::cout << "[SCENE] Failed to save" << std::endl;
	}
}

dap::GameObject* const dap::Scene::SpawnGameObject ( std::string name ) {
	dap::GameObject* newObj = new dap::GameObject ( this );
	if ( localGameObjects.contains ( name ) )
		name = name.append ( "_" + std::to_string ( newObj->GetId () ) );
	newObj->SetName ( name );
	localGameObjects.insert ( std::pair<std::string, dap::GameObject*> ( name, newObj ) );
	AllGameObjects.push_back ( newObj );
	return newObj;
}

dap::GameObject* const dap::Scene::SpawnPrefab ( std::string fileName, bool ignoreCache ) {
	dap::GameObject* newPrefab = NULL;
	nlohmann::json json = !ignoreCache && PrefabCache.count ( fileName ) ? PrefabCache[fileName] : CachePrefab ( fileName );
	if ( !json.is_null () ) {
		newPrefab = new dap::GameObject ( this );
		newPrefab->Deserialize ( &json );
		if ( newPrefab->prefabPath != fileName )
			newPrefab->prefabPath = fileName;
		if ( GetGameObject ( newPrefab->GetName () ) )
			newPrefab->SetName ( newPrefab->GetName () + "_" + std::to_string ( newPrefab->GetId () ) );
		localGameObjects.insert ( std::pair<std::string, dap::GameObject*> ( newPrefab->GetName (), newPrefab ) );
		AllGameObjects.push_back ( newPrefab );
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
