#pragma once

#include <ft2build.h>
#include <map>
#include FT_FREETYPE_H
#include "ResourceObject.h"
#include <string>
#include <vector>

namespace dap {
	class FontCharacter;
	class TextureAtlasSubTexture;

	class Font : public dap::ResourceObject {
	private:
		static const std::string GLYPHS_TO_LOAD;
		static const int GLYPH_PIXEL_SIZE;

	private:
		std::string name = "";
		FT_Library ftLib;
		std::vector<FT_Face> ftFaces;
		std::map<char, dap::FontCharacter*> charGlyphData;

	public:
		Font ( std::string name, std::string path );
		~Font ();

	public:
		bool BuildTexture ( std::string path );
		void Free ();
		dap::FontCharacter* GetChar ( char character );
		std::string GetName ();
		void SetCharTexture ( char character, dap::TextureAtlasSubTexture* texture );
	};
}
