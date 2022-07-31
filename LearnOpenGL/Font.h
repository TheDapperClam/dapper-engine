#pragma once

#include <ft2build.h>
#include FT_FREETYPE_H
#include "ResourceObject.h"
#include <string>
#include <vector>

namespace dap {
	class Font : public dap::ResourceObject {
	private:
		static const std::string GLYPHS_TO_LOAD;

	private:
		FT_Library ftLib;
		std::vector<FT_Face> ftFaces;

	public:
		Font ( std::string path );
		bool BuildTexture ( std::string path );
		void Free ();
	};
}
