#pragma once

#include <map>
#include "nlohmann/json.hpp"
#include <string>
#include <vector>

namespace dap {
	class GameObject;

	class Scene {
	private:
		static std::map<std::string, nlohmann::json> PrefabCache;

	private:
		static dap::Scene* CurrentScene;

	public:
		static nlohmann::json CachePrefab ( std::string fileName );
		static void ClearPrefabCache ();
		static dap::Scene* const GetCurrent ();
		static dap::Scene* const Load ( const std::string fileName );
		static void UnloadCurrent ();

	private:
		std::string name;
		std::string path;
		std::map<std::string, dap::GameObject*> allGameObjects;
		std::vector<dap::GameObject*> objectsToDelete;
		bool eraseOnDelete = true;

	private:
		Scene ( const std::string fileName );

	public:
		~Scene ();

	public:
		void DeleteMarkedObjects ();
		const std::vector< dap::GameObject*> GetAllGameObjectsList ();
		dap::GameObject* GetGameObject ( std::string name );
		std::string GetName ();
		std::string GetPath ();
		void MarkObjectForLateDeletion ( dap::GameObject* object );
		void RemoveGameObject ( dap::GameObject* object, bool shouldDelete = true );
		void Save ( const std::string fileName );
		dap::GameObject* const SpawnGameObject ( std::string name );
		dap::GameObject* const SpawnPrefab ( std::string fileName );
	};
}
