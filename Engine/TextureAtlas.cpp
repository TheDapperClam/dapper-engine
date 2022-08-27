#include "TextureAtlas.h"

#include "Font.h"
#include <iostream>
#include "stb_image.h"
#include "TextureGenData.h"
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

void dap::TextureAtlas::AddImage ( dap::TextureGenData* data ) {
	if ( !data ) {
		std::cout << "[TEXTUREATLAS] Failed to add image data to texture atlas" << std::endl;
		return;
	}
	allTempImageData.push_back ( data );
}

void dap::TextureAtlas::Build () {
	if ( allTempImageData.size () <= 0 )
		return;

	std::cout << "[TEXTUREATLAS] Building texture atlas" << std::endl;
	if ( !textureObject ) {
		glGenTextures ( 1, &textureObject );
		glBindTexture ( GL_TEXTURE_2D, textureObject );
		glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
		glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
		glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
		glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
	}

	unsigned int atlasWidth = SUBTEXTURE_MARGIN, atlasHeight = SUBTEXTURE_MARGIN;
	for ( const auto& tempImageData : allTempImageData ) {
		atlasWidth += tempImageData->width + SUBTEXTURE_MARGIN;
		atlasHeight = std::max ( atlasHeight, tempImageData->height + SUBTEXTURE_MARGIN * 2 );
	}

	float atlasX = SUBTEXTURE_MARGIN, atlasY = SUBTEXTURE_MARGIN;
	ClearSubTextures ();
	glTexImage2D ( GL_TEXTURE_2D, 0, GL_RGBA, atlasWidth, atlasHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	for ( const auto& tempImageData : allTempImageData ) {
		glTexSubImage2D ( GL_TEXTURE_2D, 0, atlasX, atlasY, tempImageData->width, tempImageData->height, tempImageData->pixelMode, GL_UNSIGNED_BYTE, tempImageData->bytes );
		GLuint error = glGetError ();
		if ( error )
			std::cout << "[ERROR::TEXTUREATLAS] with sub texture " << tempImageData->name << ": " << error << std::endl;
		dap::TextureAtlasSubTexture* subTexture = new dap::TextureAtlasSubTexture {
			tempImageData->name,
			atlasX / atlasWidth,
			atlasY / atlasHeight,
			( (float) tempImageData->width + SUBTEXTURE_MARGIN ) / atlasWidth,
			( (float) tempImageData->height + SUBTEXTURE_MARGIN ) / atlasHeight
		};
		atlasX += tempImageData->width + SUBTEXTURE_MARGIN;
		if ( tempImageData->font )
			tempImageData->font->SetCharTexture ( tempImageData->name[0], subTexture );
		else
			allSubTextures.insert ( std::pair<std::string, dap::TextureAtlasSubTexture*> ( subTexture->name, subTexture ) );
		delete tempImageData;
	}
	allTempImageData.clear ();
	width = atlasWidth;
	height = atlasHeight;
}

void dap::TextureAtlas::ClearSubTextures () {
	std::cout << "[TEXTUREATLAS] Clearing sub textures" << std::endl;
	for ( const auto& subTexturePair : allSubTextures ) {
		delete subTexturePair.second;
	}
	allSubTextures.clear ();
}

std::map<std::string, dap::TextureAtlasSubTexture*>* const dap::TextureAtlas::GetAllSubTextures () {
	return &allSubTextures;
}

dap::TextureAtlas* const dap::TextureAtlas::GetMainAtlas () {
	return &mainAtlas;
}

dap::TextureAtlasSubTexture* const dap::TextureAtlas::GetSubTexture ( std::string name ) {
	if ( !allSubTextures.contains ( name ) )
		return NULL;
	return allSubTextures.at ( name );
}

const GLuint* dap::TextureAtlas::GetTextureObject () const {
	return &textureObject;
}

int dap::TextureAtlas::GetWidth () const {
	return width;
}

int dap::TextureAtlas::GetHeight () const {
	return height;
}
