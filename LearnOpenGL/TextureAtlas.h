#pragma once

#include "glad/glad.h"
#include <map>
#include "ResourceObject.h"
#include <string>
#include <vector>

namespace dap {
	struct TextureAtlasSubTexture;

	class TextureAtlas : public dap::ResourceObject {
	private:
		static const int SUBTEXTURE_MARGIN = 1;

	private:
		static dap::TextureAtlas mainAtlas;

	private:
		GLuint textureObject = 0;
		std::vector<std::string> allImagePaths = {};
		std::map<std::string, dap::TextureAtlasSubTexture*> allSubTextures {};
		dap::TextureAtlasSubTexture* wholeTexture;

	public:
		static dap::TextureAtlas* const GetMainAtlas ();

	public:
		~TextureAtlas ();

	public:
		void AddImage ( std::string path );
		void Build ();
		dap::TextureAtlasSubTexture* const GetSubTexture ( std::string name ) const;
	};
}
