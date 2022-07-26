#include "TextureImporter.h"
#include <iostream>
#include <filesystem>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include "TextureAtlas.h"
#include "TextureAtlasImageData.h"

std::vector<std::string> dap::TextureImporter::GetExtensions () {
	return { ".jpg", ".png", ".bmp" };
}

void dap::TextureImporter::Import ( std::string key, std::string path ) {
	if ( dap::TextureAtlas* atlas = dap::TextureAtlas::GetMainAtlas () ) {
		std::cout << "Importing texture: " + key << std::endl;
		int nrChannels = 0;
		int imageWidth = 0, imageHeight = 0;
		stbi_set_flip_vertically_on_load ( true );
		unsigned char* data = stbi_load ( path.c_str (), &imageWidth, &imageHeight, &nrChannels, STBI_rgb_alpha );
		allImageData.push_back ( data );
		dap::TextureAtlasImageData* tempImageData = new dap::TextureAtlasImageData {
			key,
			imageWidth,
			imageHeight,
			data
		};
		atlas->AddImage ( tempImageData );
	}
}

void dap::TextureImporter::ImportingComplete () {
	std::cout << "Texture importing complete" << std::endl;
	for ( const auto& data : allImageData ) {
		stbi_image_free ( data );
	}
	allImageData.clear ();
}
