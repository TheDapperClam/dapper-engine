#include "TextureImporter.h"
#include <iostream>
#include <filesystem>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include "TextureAtlas.h"
#include "TextureGenData.h"

std::vector<std::string> dap::TextureImporter::GetExtensions () {
	return { ".jpg", ".png", ".bmp" };
}

void dap::TextureImporter::Import ( std::string key, std::string path ) {
	if ( dap::TextureAtlas* atlas = dap::TextureAtlas::GetMainAtlas () ) {
		std::cout << "Importing texture: " + key << std::endl;
		int nrChannels = 0;
		int imageWidth = 0, imageHeight = 0;
		unsigned char* data = stbi_load ( path.c_str (), &imageWidth, &imageHeight, &nrChannels, STBI_rgb_alpha );
		allImageData.push_back ( data );
		dap::TextureGenData* tempImageData = new dap::TextureGenData {
			key,
			(unsigned int) imageWidth,
			(unsigned int) imageHeight,
			data,
			GL_RGBA
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
