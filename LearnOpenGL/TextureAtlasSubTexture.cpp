#include "TextureAtlasSubTexture.h"

#include "glm/glm.hpp"

const glm::vec2* dap::TextureAtlasSubTexture::GetTexCoords () {
	glm::vec2 coords[4];
	coords[0] = glm::vec2 ( x, y + height );
	coords[1] = glm::vec2 ( x + width, y + height );
	coords[2] = glm::vec2 ( x + width, y );
	coords[3] = glm::vec2 ( x, y );
	return coords;
}
