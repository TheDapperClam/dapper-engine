#pragma once

#include "nlohmann/json.hpp"
#include <string>
#include <unordered_map>
#include <vector>

namespace dap {
	class GameObject;

	class Scene {
		friend class Game;
		friend class Rendering;

	private:
		static std::vector<dap::GameObject*> AllGameObjects;
		static dap::Scene* CurrentScene;
		static std::unordered_map<std::string, nlohmann::json> PrefabCache;
		static std::string SceneToLateLoad;
		static bool WantsToLateLoad;

	public:
		static nlohmann::json CachePrefab ( std::string fileName );
		static void ClearPrefabCache ();
		static const std::vector<dap::GameObject*>* GetAllGameObjects ();
		static dap::Scene* const GetCurrent ();
		static void LoadLate ( const std::string fileName );
		static void UnloadCurrent ();

	private:
		static void BeginLateLoad ();
		static dap::Scene* const LoadInstant ( const std::string fileName );

	private:
		std::string name;
		std::string path;
		std::unordered_map<std::string, dap::GameObject*> localGameObjects;
		std::vector<dap::GameObject*> objectsToDelete;
		bool eraseOnDelete = true;

	private:
		Scene ( const std::string fileName );

	public:
		~Scene ();

	public:
		void DeleteMarkedObjects ();
		dap::GameObject* GetGameObject ( std::string name );
		const std::unordered_map<std::string, dap::GameObject*>* GetLocalGameObjects ();
		std::string GetName ();
		std::string GetPath ();
		void MarkObjectForLateDeletion ( dap::GameObject* object );
		void RemoveGameObject ( dap::GameObject* object, bool shouldDelete = true );
		void Save ( const std::string fileName );
		dap::GameObject* const SpawnGameObject ( std::string name );
		dap::GameObject* const SpawnPrefab ( std::string fileName, bool ignoreCache = false );
	};
}
