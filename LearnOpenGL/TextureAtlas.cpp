#include "TextureAtlas.h"

#include <filesystem>
#include <stb_image.h>

dap::TextureAtlas::TextureAtlas ( int width, int height ) {
	glGenTextures ( 1, &textureObject );
	glBindTexture ( GL_TEXTURE_2D, textureObject );
	glTexImage2D ( GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL );

	glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
	glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
	glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
	glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );

	glBindTexture ( GL_TEXTURE_2D, NULL );
}

dap::TextureAtlas::~TextureAtlas () {
	for ( const auto& subTexturePair : allSubTextures ) {
		delete subTexturePair.second;
	}
	glDeleteTextures ( 1, &textureObject );
}

void dap::TextureAtlas::AddImage ( std::string path ) {
	if ( path.empty () || !std::filesystem::exists ( path ) )
		return;
	allImagePaths.push_back ( path );
}

void dap::TextureAtlas::Build () {
	int atlasWidth = 0, atlasHeight = 0;
	int atlasX = 0, atlasY = 0;

	for ( const auto& path : allImagePaths ) {
		int nrChannels = 0;
		int imageWidth = 0, imageHeight = 0;
		stbi_set_flip_vertically_on_load ( true );
		unsigned char* data = stbi_load ( path.c_str (), &imageWidth, &imageHeight, &nrChannels, STBI_rgb_alpha );

		const std::string fileName = std::filesystem::path ( path ).filename ().string ();
		const unsigned int fileExtensionStart = fileName.find ( '.' );
		const std::string fileNameWithoutExtension = fileName.substr ( 0, fileExtensionStart );
		dap::TextureAtlasSubTexture* subTexture = new dap::TextureAtlasSubTexture ( imageWidth, imageHeight, data );

		allSubTextures.insert ( std::pair<std::string, dap::TextureAtlasSubTexture*> ( fileNameWithoutExtension, subTexture ) );
	}

	glBindTexture ( GL_TEXTURE_2D, textureObject );
	for ( const auto& subTexturePair : allSubTextures ) {
		dap::TextureAtlasSubTexture* subTexture = subTexturePair.second;
		glTexSubImage2D ( GL_TEXTURE_2D, 0, atlasX, atlasY, subTexture->width, subTexture->height, GL_RGBA, GL_UNSIGNED_BYTE, subTexture->tempImageData );
		stbi_image_free ( subTexture->tempImageData );
		subTexture->x = atlasX;
		subTexture->y = atlasY;
		subTexture->tempImageData = NULL;
	}
	glBindTexture ( GL_TEXTURE_2D, NULL );
}

const dap::TextureAtlasSubTexture* dap::TextureAtlas::GetSubTexture ( std::string name ) {
	if ( !allSubTextures.contains ( name ) )
		return NULL;
	return allSubTextures[name];
}
