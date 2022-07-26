#include "TextureImporter.h"
#include <iostream>
#include <filesystem>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include "TextureAtlas.h"
#include "TextureAtlasImageData.h"

std::vector<std::string> dap::TextureImporter::GetExtensions () const {
	return { ".jpg", ".png", ".bmp" };
}

void dap::TextureImporter::Import ( std::string key, std::string path ) const {
	if ( dap::TextureAtlas* atlas = dap::TextureAtlas::GetMainAtlas () ) {
		std::cout << "Importing texture: " + key << std::endl;
		int nrChannels = 0;
		int imageWidth = 0, imageHeight = 0;
		stbi_set_flip_vertically_on_load ( true );
		unsigned char* data = stbi_load ( path.c_str (), &imageWidth, &imageHeight, &nrChannels, STBI_rgb_alpha );
		dap::TextureAtlasImageData* tempImageData = new dap::TextureAtlasImageData {
			key,
			imageWidth,
			imageHeight,
			data
		};
		atlas->AddImage ( tempImageData );
	}
}
