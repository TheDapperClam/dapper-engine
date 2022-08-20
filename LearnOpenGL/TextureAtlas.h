#pragma once

#include "glad/glad.h"
#include <map>
#include <string>
#include <vector>

namespace dap {
	struct TextureGenData;
	struct TextureAtlasSubTexture;

	class TextureAtlas {
	private:
		static const int SUBTEXTURE_MARGIN = 1;

	private:
		static dap::TextureAtlas mainAtlas;

	private:
		GLuint textureObject = 0;
		std::vector<dap::TextureGenData*> allTempImageData = {};
		std::map<std::string, dap::TextureAtlasSubTexture*> allSubTextures {};
		int width = 0, height = 0;

	public:
		static dap::TextureAtlas* const GetMainAtlas ();

	public:
		~TextureAtlas ();

	public:
		void AddImage ( dap::TextureGenData* data );
		void Build ();
		dap::TextureAtlasSubTexture* const GetSubTexture ( std::string name ) const;
		const GLuint* GetTextureObject () const;
		int GetWidth () const;
		int GetHeight () const;

	private:
		void ClearSubTextures ();
	};
}
