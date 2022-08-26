#pragma once

#include <string>
#include "glm/glm.hpp"

namespace dap {
	struct TextureAtlasSubTexture {
	public:
		std::string name = "";
		float x = 0.0f, y = 0.0f;
		float width = 0.0f, height = 0.0f;

	public:
		const glm::vec2* GetTexCoords ();
	};
}
