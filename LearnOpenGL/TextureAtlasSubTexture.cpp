#include "TextureAtlasSubTexture.h"

#include "glm/glm.hpp"

dap::TextureAtlasSubTexture::TextureAtlasSubTexture ( std::string name, float width, float height, unsigned char* tempImageData ) {
	this->name = name;
	this->width = width;
	this->height = height;
	this->tempImageData = tempImageData;
}

const glm::vec2* dap::TextureAtlasSubTexture::GetTexCoords () {
	glm::vec2 coords[4];
	coords[0] = glm::vec2 ( x, y + height );
	coords[1] = glm::vec2 ( x + width, y + height );
	coords[2] = glm::vec2 ( x + width, y );
	coords[3] = glm::vec2 ( x, y );
	return coords;
}
