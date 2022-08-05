#include "Font.h"
#include "FontCharacter.h"
#include "glm/glm.hpp"
#include <iostream>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>
#include "TextureAtlas.h"
#include "TextureGenData.h"
#include "TextureAtlasSubTexture.h"

const std::string dap::Font::GLYPHS_TO_LOAD = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890 !@#$%^&*()[]{}<>/\\|'\":;_+-=:,.?`~";

dap::Font::Font ( std::string name, std::string path ) {
	this->name = name;
	BuildTexture ( path );
}

dap::Font::~Font () {
	for ( const auto& data : charGlyphData ) {
		delete data.second;
	}
	charGlyphData.clear ();
}

bool dap::Font::BuildTexture ( std::string path ) {
	// TODO: only init FreeType once?
	if ( FT_Init_FreeType ( &ftLib ) ) {
		std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
		return false;
	}
	
	for ( const char& glyphToLoad : GLYPHS_TO_LOAD ) {
		FT_Face ftFace;
		if ( FT_New_Face ( ftLib, path.c_str (), 0, &ftFace ) ) {
			std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
			continue;
		}
		FT_Set_Pixel_Sizes ( ftFace, 0, 72 );
		FT_Select_Charmap ( ftFace, ft_encoding_unicode );
		ftFaces.push_back ( ftFace );

		if ( FT_Load_Char ( ftFace, glyphToLoad, FT_LOAD_RENDER ) ) {
			std::cout << "ERROR:FREETYPE: Failed to load Glyph: " << glyphToLoad << " for: " << path << std::endl;
			continue;
		}

		dap::FontCharacter* charData = new dap::FontCharacter {
			glm::ivec2 ( ftFace->glyph->bitmap.width, ftFace->glyph->bitmap.rows ),
			glm::ivec2 ( ftFace->glyph->bitmap_left, ftFace->glyph->bitmap_top ),
			(unsigned int) ftFace->glyph->advance.x
		};
		charGlyphData.insert ( std::pair<char, dap::FontCharacter*> ( glyphToLoad, charData ) );

		dap::TextureGenData* tempImageData = new dap::TextureGenData {
			std::string ( 1, glyphToLoad ),
			ftFace->glyph->bitmap.width,
			ftFace->glyph->bitmap.rows,
			ftFace->glyph->bitmap.buffer,
			GL_RED,
			this
		};
		dap::TextureAtlas::GetMainAtlas ()->AddImage ( tempImageData );
	}
	return true;
}

void dap::Font::Free () {
	for ( const auto& ftFace : ftFaces ) {
		FT_Done_Face ( ftFace );
	}
	ftFaces.clear ();
	FT_Done_FreeType ( ftLib );
}

dap::FontCharacter* dap::Font::GetChar ( char character ) {
	if ( !charGlyphData.contains ( character ) )
		return NULL;
	return charGlyphData[character];
}

std::string dap::Font::GetName () {
	return name;
}

void dap::Font::SetCharTexture ( char character, dap::TextureAtlasSubTexture* texture ) {
	if ( !charGlyphData.contains ( character ) )
		return;
	charGlyphData[character]->texture = texture;
}
