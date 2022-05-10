#pragma once

#include "GameObject.h"
#include <map>
#include <string>
#include <vector>

namespace dap {
	class Scene {
	public:
		Scene ( const std::string fileName );
		~Scene ();
		static void ClearAll ();
		static const dap::Scene* GetScene ( const std::string id );
		static void Save ( const dap::Scene* scene, const std::string fileName );
	private:
		static std::map<const std::string, dap::Scene* const> loadedScenes;
		std::string name;
		std::vector<dap::GameObject*> allGameObjects;
		bool eraseOnDelete = true;
	};
}
