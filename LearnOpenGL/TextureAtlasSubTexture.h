#pragma once

#include <string>
#include "glm/glm.hpp"

namespace dap {
	struct TextureAtlasSubTexture {
	public:
		std::string name = "";
		float x = 0, y = 0;
		float width = 0, height = 0;
		unsigned char* tempImageData = NULL;

	public:
		TextureAtlasSubTexture ( std::string name, float width, float height, unsigned char* tempImageData );

	public:
		const glm::vec2* GetTexCoords ();
	};
}
