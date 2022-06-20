#include "Texture.h"

#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

dap::Texture* dap::Texture::currentBoundTexture = NULL;

dap::Texture::Texture ( const std::string fileName, const std::string name ) {
	this->name = name;
	int nrChannels;
	stbi_set_flip_vertically_on_load ( true );
	unsigned char* data = stbi_load ( fileName.c_str (), &width, &height, &nrChannels, STBI_rgb_alpha );

	if ( data ) {
		glGenTextures ( 1, &textureObject );
		glBindTexture ( GL_TEXTURE_2D, textureObject );
		glTexImage2D ( GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data );
		glGenerateMipmap ( GL_TEXTURE_2D );

		glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
		glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
		glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR );
		glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
	} else {
		std::cout << "[ERROR] loading image " << fileName << " for reason: " << stbi_failure_reason () <<  std::endl;
	}

	glBindTexture ( GL_TEXTURE_2D, NULL );
	stbi_image_free ( data );
}

dap::Texture::~Texture () {
	glDeleteTextures ( 1, &textureObject );
	textureObject = 0;
	width = 0;
	height = 0;
}

const std::string* dap::Texture::GetName () {
	return &name;
}

void dap::Texture::Use () {
	if ( currentBoundTexture == this )
		return;
	currentBoundTexture = this;
	glBindTexture ( GL_TEXTURE_2D, textureObject );
}
