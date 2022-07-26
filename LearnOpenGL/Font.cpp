#include "Font.h"
#include <iostream>
#include <ft2build.h>
#include FT_FREETYPE_H
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

const std::string dap::Font::GLYPHS_TO_LOAD = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890!@#$%^&*()[]{}<>/\\|'\":;_+-=:,.?`~";

dap::Font::Font ( std::string path ) {
	BuildTexture ( path );
}

bool dap::Font::BuildTexture ( std::string path ) {
	// TODO: only init FreeType once?
	FT_Library ftLib;
	if ( FT_Init_FreeType ( &ftLib ) ) {
		std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
		return false;
	}
	
	FT_Face ftFace;
	if ( FT_New_Face ( ftLib, path.c_str (), 0, &ftFace ) ) {
		std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
		return false;
	}
	
	FT_Set_Pixel_Sizes ( ftFace, 0, 72 );
	for ( int i = 0; i < GLYPHS_TO_LOAD.size (); i++ ) {
		char c = GLYPHS_TO_LOAD[i];
		if ( FT_Load_Char ( ftFace, c, FT_LOAD_RENDER ) ) {
			std::cout << "ERROR:FREETYPE: Failed to load Glyph: " << c << " for: " << path << std::endl;
			continue;
		}

		std::cout << "Loaded Glyph: " << c << " for: " << path << std::endl;
	}

	FT_Done_Face ( ftFace );
	FT_Done_FreeType ( ftLib );
	return true;
}
