#pragma once

#include "glm/glm.hpp"

namespace dap {
	struct TextureAtlasSubTexture;

	struct FontCharacter {
		glm::ivec2 size;
		glm::ivec2 bearing;
		unsigned int advance;
		dap::TextureAtlasSubTexture* texture = NULL;
	};
}
