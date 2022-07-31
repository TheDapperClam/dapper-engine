#include "TextureAtlas.h"

#include <iostream>
#include "stb_image.h"
#include "TextureAtlasImageData.h"
#include "TextureAtlasSubTexture.h" 

dap::TextureAtlas dap::TextureAtlas::mainAtlas = dap::TextureAtlas ();

dap::TextureAtlas::~TextureAtlas () {
	for ( const auto& tempImageData : allTempImageData ) {
		delete tempImageData;
	}
	allTempImageData.clear ();
	ClearSubTextures ();
	glDeleteTextures ( 1, &textureObject );
}

void dap::TextureAtlas::AddImage ( dap::TextureAtlasImageData* data ) {
	if ( !data ) {
		std::cout << "Failed to add image data to texture atlas" << std::endl;
		return;
	}
	allTempImageData.push_back ( data );
}

void dap::TextureAtlas::Build () {
	if ( allTempImageData.size () <= 0 )
		return;

	std::cout << "Building texture atlas" << std::endl;
	unsigned int atlasWidth = 0, atlasHeight = 0;
	for ( const auto& tempImageData : allTempImageData ) {
		atlasWidth += tempImageData->width;
		atlasHeight = std::max ( atlasHeight, tempImageData->height );
	}
	atlasWidth += SUBTEXTURE_MARGIN + allSubTextures.size ();
	atlasHeight += SUBTEXTURE_MARGIN * 2;

	if ( !textureObject ) {
		glGenTextures ( 1, &textureObject );
		glBindTexture ( GL_TEXTURE_2D, textureObject );

		glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
		glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
		glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
		glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
	}

	float atlasX = SUBTEXTURE_MARGIN, atlasY = SUBTEXTURE_MARGIN;
	ClearSubTextures ();
	glTexImage2D ( GL_TEXTURE_2D, 0, GL_RGB, atlasWidth, atlasHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL );
	for ( const auto& tempImageData : allTempImageData ) {
		glTexSubImage2D ( GL_TEXTURE_2D, 0, atlasX, atlasY, tempImageData->width, tempImageData->height, tempImageData->pixelMode, GL_UNSIGNED_BYTE, tempImageData->bytes );
		dap::TextureAtlasSubTexture* subTexture = new dap::TextureAtlasSubTexture {
			tempImageData->name,
			atlasX / atlasWidth,
			atlasY / atlasHeight,
			( (float) tempImageData->width + SUBTEXTURE_MARGIN ) / atlasWidth,
			( (float) tempImageData->height + SUBTEXTURE_MARGIN ) / atlasHeight
		};
		atlasX += tempImageData->width + SUBTEXTURE_MARGIN;
		allSubTextures.insert ( std::pair<std::string, dap::TextureAtlasSubTexture*> ( subTexture->name, subTexture ) );
		delete tempImageData;
	}
	allTempImageData.clear ();
}

void dap::TextureAtlas::ClearSubTextures () {
	for ( const auto& subTexturePair : allSubTextures ) {
		delete subTexturePair.second;
	}
	allSubTextures.clear ();
}

dap::TextureAtlas* const dap::TextureAtlas::GetMainAtlas () {
	return &mainAtlas;
}

dap::TextureAtlasSubTexture* const dap::TextureAtlas::GetSubTexture ( std::string name ) const {
	if ( !allSubTextures.contains ( name ) )
		return NULL;
	return allSubTextures.at ( name );
}
