#pragma once

#include <map>
#include <string>

namespace dap {
	class GameObject;

	class Scene {
	private:
		static std::map<const std::string, dap::Scene* const> loadedScenes;

	public:
		static void ClearAll ();
		static dap::Scene* GetScene ( const std::string id );
		static void Save ( const dap::Scene* scene, const std::string fileName );

	private:
		std::string name;
		std::map<std::string, dap::GameObject*> allGameObjects;
		bool eraseOnDelete = true;

	public:
		Scene ( const std::string fileName );
		~Scene ();

	public:
		void DeleteGameObject ( dap::GameObject* object );
		dap::GameObject* GetGameObject ( std::string name );
		std::string GetName ();
		dap::GameObject* SpawnGameObject ( std::string name );
	};
}
