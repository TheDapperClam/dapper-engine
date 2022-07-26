#pragma once

#include "glad/glad.h"
#include <map>
#include <string>
#include <vector>

namespace dap {
	struct TextureAtlasImageData;
	struct TextureAtlasSubTexture;

	class TextureAtlas {
	private:
		static const int SUBTEXTURE_MARGIN = 1;

	private:
		static dap::TextureAtlas mainAtlas;

	private:
		GLuint textureObject = 0;
		std::vector<dap::TextureAtlasImageData*> allTempImageData = {};
		std::map<std::string, dap::TextureAtlasSubTexture*> allSubTextures {};

	public:
		static dap::TextureAtlas* const GetMainAtlas ();

	public:
		~TextureAtlas ();

	public:
		void AddImage ( dap::TextureAtlasImageData* data );
		void Build ();
		dap::TextureAtlasSubTexture* const GetSubTexture ( std::string name ) const;

	private:
		void ClearSubTextures ();
	};
}
