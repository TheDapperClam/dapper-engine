#include "TextureAtlasSubTexture.h"

#include "glm/glm.hpp"

const glm::vec2* dap::TextureAtlasSubTexture::GetTexCoords () {
	glm::vec2 coords[4];
	coords[0] = glm::vec2 ( x, y );
	coords[1] = glm::vec2 ( x + width, y );
	coords[2] = glm::vec2 ( x + width, y + height );
	coords[3] = glm::vec2 ( x, y + height );
	return coords;
}
