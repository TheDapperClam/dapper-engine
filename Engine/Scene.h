#pragma once

#include "JSONObject.h"
#include "nlohmann/json.hpp"
#include <string>
#include <unordered_map>
#include <vector>

namespace dap {
	class GameObject;

	class Scene : public dap::JSONObject {
		FUNC_GET_FILE_EXTENSION(".dsn")

		friend class Editor;
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
		static void LoadLate ( const std::string path );
		static void UnloadCurrent ();

	private:
		static void BeginLateLoad ();
		static dap::Scene* const LoadInstant ( const std::string path );

	private:
		std::string name;
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
		void MarkObjectForLateDeletion ( dap::GameObject* object );
		void RemoveGameObject ( dap::GameObject* object, bool shouldDelete = true );
		dap::GameObject* const SpawnGameObject ( std::string name );
		dap::GameObject* const SpawnPrefab ( std::string fileName, bool ignoreCache = false );

	public:
		virtual void Deserialize ( const nlohmann::json* json ) override;
		virtual nlohmann::json Serialize () override;
	};
}
