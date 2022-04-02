#pragma once

#include <fstream>
#include "GameObject.h"
#include "glm/glm.hpp"
#include <map>
#include "nlohmann/json.hpp"
#include <string>
#include <vector>

namespace dap {
	class Scene {
	public:
		Scene ( const std::string fileName, const std::string id );
		~Scene ();
		static void ClearAll ();
		static const dap::Scene* GetScene ( const std::string id );
	private:
		static std::map<const std::string, dap::Scene* const> loadedScenes;
		const std::string name;
		std::vector<dap::GameObject*> allGameObjects;
	};
}
