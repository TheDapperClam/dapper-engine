#include "Font.h"
#include <iostream>
#include <ft2build.h>
#include FT_FREETYPE_H

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
	
	FT_Set_Pixel_Sizes ( ftFace, 0, 48 );
	
	if ( FT_Load_Char ( ftFace, 'X', FT_LOAD_RENDER ) ) {
		std::cout << "ERROR:FREETYPE: Failed to load Glyph" << std::endl;
		return false;
	}

	return true;
}
