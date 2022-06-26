#pragma once

#include "glad/glad.h"
#include <map>
#include <string>
#include <vector>

namespace dap {
	struct TextureAtlasSubTexture {
	public:
		TextureAtlasSubTexture ( int width, int height, unsigned char* tempImageData ) {
			this->width = width;
			this->height = height;
			this->tempImageData = tempImageData;
		}

	public:
		int x = 0, y = 0;
		int width = 0, height = 0;
		unsigned char* tempImageData = NULL;
	};

	class TextureAtlas {
	private:
		GLuint textureObject = 0;
		std::vector<std::string> allImagePaths = {};
		std::map<std::string, dap::TextureAtlasSubTexture*> allSubTextures {};

	public:
		TextureAtlas ( int width, int height );
		~TextureAtlas ();

	public:
		void AddImage ( std::string path );
		void Build ();
		const dap::TextureAtlasSubTexture* GetSubTexture ( std::string name );
	};
}
