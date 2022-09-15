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
const int dap::Font::GLYPH_PIXEL_SIZE = 72;
std::unordered_map<std::string, dap::Font*> dap::Font::AllFonts = {};

dap::Font::Font ( std::string name, std::string path ) {
	this->name = name;
	BuildTexture ( path );
	AllFonts.insert ( std::pair<std::string, dap::Font*> ( name, this ) );
}

dap::Font::~Font () {
	if ( eraseOnDelete )
		AllFonts.erase ( name );
	for ( const auto& data : charGlyphData ) {
		delete data.second;
	}
	charGlyphData.clear ();
}

bool dap::Font::BuildTexture ( std::string path ) {
	if ( FT_Init_FreeType ( &ftLib ) ) {
		std::cout << "[ERROR::FREETYPE] Could not init FreeType Library" << std::endl;
		return false;
	}
	
	for ( const char& glyphToLoad : GLYPHS_TO_LOAD ) {
		FT_Face ftFace;
		if ( FT_New_Face ( ftLib, path.c_str (), 0, &ftFace ) ) {
			std::cout << "[ERROR::FREETYPE] Failed to load font" << std::endl;
			continue;
		}
		FT_Set_Pixel_Sizes ( ftFace, 0, GLYPH_PIXEL_SIZE );
		FT_Select_Charmap ( ftFace, ft_encoding_unicode );
		ftFaces.push_back ( ftFace );

		if ( FT_Load_Char ( ftFace, glyphToLoad, FT_LOAD_RENDER ) ) {
			std::cout << "[ERROR:FREETYPE] Failed to load Glyph: " << glyphToLoad << " for: " << path << std::endl;
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

void dap::Font::ClearFonts () {
	std::cout << "[FONT] Clearing fonts" << std::endl;
	for ( const auto& fontPair : AllFonts ) {
		fontPair.second->eraseOnDelete = false;
		delete fontPair.second;
	}
	AllFonts.clear ();
}

void dap::Font::Free () {
	std::cout << "[FONT] Freeing faces: " << name << std::endl;
	for ( const auto& ftFace : ftFaces ) {
		FT_Done_Face ( ftFace );
	}
	ftFaces.clear ();
	FT_Done_FreeType ( ftLib );
}

std::unordered_map<std::string, dap::Font*>* const dap::Font::GetAllFonts () {
	return &AllFonts;
}

dap::Font* const dap::Font::GetFont ( std::string key ) {
	if ( !AllFonts.count ( key ) )
		return NULL;
	return AllFonts[key];
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
