#pragma once

#include "GameObject.h"
#include <map>
#include <string>
#include <map>

namespace dap {
	class Scene {
	public:
		static void ClearAll ();
		static dap::Scene* GetScene ( const std::string id );
		static void Save ( const dap::Scene* scene, const std::string fileName );

	public:
		Scene ( const std::string fileName );
		~Scene ();
		void DestroyGameObject ( dap::GameObject* object );
		dap::GameObject* GetGameObject ( std::string name );
		dap::GameObject* SpawnGameObject ( std::string name );

	private:
		static std::map<const std::string, dap::Scene* const> loadedScenes;
		std::string name;
		std::map<std::string, dap::GameObject*> allGameObjects;
		bool eraseOnDelete = true;
	};
}
