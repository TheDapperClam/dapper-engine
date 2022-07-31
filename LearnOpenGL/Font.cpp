#include "Font.h"
#include <iostream>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>
#include "TextureAtlas.h"
#include "TextureAtlasImageData.h"

const std::string dap::Font::GLYPHS_TO_LOAD = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890!@#$%^&*()[]{}<>/\\|'\":;_+-=:,.?`~";

dap::Font::Font ( std::string path ) {
	BuildTexture ( path );
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
		ftFaces.push_back ( ftFace );

		if ( FT_Load_Char ( ftFace, glyphToLoad, FT_LOAD_RENDER ) ) {
			std::cout << "ERROR:FREETYPE: Failed to load Glyph: " << glyphToLoad << " for: " << path << std::endl;
			continue;
		}

		dap::TextureAtlasImageData* tempImageData = new dap::TextureAtlasImageData {
			std::string ( 1, glyphToLoad ),
			ftFace->glyph->bitmap.width,
			ftFace->glyph->bitmap.rows,
			ftFace->glyph->bitmap.buffer,
			GL_RED
		};
		dap::TextureAtlas::GetMainAtlas ()->AddImage ( tempImageData );
		std::cout << "Loaded Glyph: " << glyphToLoad << " for: " << path << std::endl;
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
