#include "TextureAtlas.h"

#include <filesystem>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include "TextureAtlasSubTexture.h" 

dap::TextureAtlas dap::TextureAtlas::mainAtlas = dap::TextureAtlas ();

dap::TextureAtlas::~TextureAtlas () {
	for ( const auto& subTexturePair : allSubTextures ) {
		delete subTexturePair.second;
	}
	delete wholeTexture;
	glDeleteTextures ( 1, &textureObject );
}

void dap::TextureAtlas::AddImage ( std::string path ) {
	if ( path.empty () || !std::filesystem::exists ( path ) )
		return;
	allImagePaths.push_back ( path );
}

void dap::TextureAtlas::Build () {
	if ( allImagePaths.size () <= 0 )
		return;

	int atlasWidth = 0, atlasHeight = 0;
	for ( const auto& path : allImagePaths ) {
		int nrChannels = 0;
		int imageWidth = 0, imageHeight = 0;
		stbi_set_flip_vertically_on_load ( true );
		unsigned char* data = stbi_load ( path.c_str (), &imageWidth, &imageHeight, &nrChannels, STBI_rgb_alpha );
		atlasWidth += imageWidth;
		atlasHeight = std::max ( atlasHeight, imageHeight );
	
		const std::string fileName = std::filesystem::path ( path ).filename ().string ();
		const unsigned int fileExtensionStart = fileName.find ( '.' );
		const std::string fileNameWithoutExtension = fileName.substr ( 0, fileExtensionStart );
		dap::TextureAtlasSubTexture* subTexture = new dap::TextureAtlasSubTexture ( fileNameWithoutExtension, imageWidth, imageHeight, data );
	
		allSubTextures.insert ( std::pair<std::string, dap::TextureAtlasSubTexture*> ( fileNameWithoutExtension, subTexture ) );
	}
	atlasWidth += allSubTextures.size () + SUBTEXTURE_MARGIN;
	atlasHeight += SUBTEXTURE_MARGIN * 2;
	wholeTexture = new dap::TextureAtlasSubTexture ( "", atlasWidth, atlasHeight, NULL );

	if ( !textureObject ) {
		glGenTextures ( 1, &textureObject );
		glBindTexture ( GL_TEXTURE_2D, textureObject );

		glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
		glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
		glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
		glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
	}

	int atlasX = SUBTEXTURE_MARGIN, atlasY = SUBTEXTURE_MARGIN;
	glTexImage2D ( GL_TEXTURE_2D, 0, GL_RGBA, atlasWidth, atlasHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL );
	for ( const auto& subTexturePair : allSubTextures ) {
		dap::TextureAtlasSubTexture* subTexture = subTexturePair.second;
		glTexSubImage2D ( GL_TEXTURE_2D, 0, atlasX, atlasY, subTexture->width, subTexture->height, GL_RGBA, GL_UNSIGNED_BYTE, subTexture->tempImageData );
		stbi_image_free ( subTexture->tempImageData );
		atlasX += subTexture->width + SUBTEXTURE_MARGIN;
		subTexture->width /= atlasWidth;
		subTexture->height /= atlasHeight;
		subTexture->x = atlasX / atlasWidth;
		subTexture->y = 1.0f / atlasHeight;
		subTexture->tempImageData = NULL;
	}
}

dap::TextureAtlas* const dap::TextureAtlas::GetMainAtlas () {
	return &mainAtlas;
}

dap::TextureAtlasSubTexture* const dap::TextureAtlas::GetSubTexture ( std::string name ) const {
	if ( !allSubTextures.contains ( name ) )
		return NULL;
	return allSubTextures.at ( name );
}
