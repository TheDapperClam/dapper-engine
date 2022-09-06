#pragma once

#include <unordered_map>
#include "nlohmann/json.hpp"
#include <string>
#include <vector>

namespace dap {
	class GameObject;

	class Scene {
		friend class Game;
		friend class Rendering;

	private:
		static std::unordered_map<std::string, nlohmann::json> PrefabCache;

	private:
		static dap::Scene* CurrentScene;
		static std::string SceneToLateLoad;
		static bool WantsToLateLoad;

	public:
		static nlohmann::json CachePrefab ( std::string fileName );
		static void ClearPrefabCache ();
		static dap::Scene* const GetCurrent ();
		static void Load ( const std::string fileName );
		static void UnloadCurrent ();

	private:
		static void BeginLateLoad ();
		static dap::Scene* const LoadInstant ( const std::string fileName );

	private:
		std::string name;
		std::string path;
		std::unordered_map<std::string, dap::GameObject*> allGameObjects;
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
