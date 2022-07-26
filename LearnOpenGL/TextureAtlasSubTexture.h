#pragma once

#include <string>
#include "glm/glm.hpp"

namespace dap {
	struct TextureAtlasSubTexture {
	public:
		std::string name = "";
		float x = 0, y = 0;
		float width = 0, height = 0;

	public:
		const glm::vec2* GetTexCoords ();
	};
}
