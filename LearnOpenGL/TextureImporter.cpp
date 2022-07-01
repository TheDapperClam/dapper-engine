#include "TextureImporter.h"
#include <iostream>
#include "TextureAtlas.h"

std::vector<std::string> dap::TextureImporter::GetExtensions () const {
	return { ".jpg", ".png", ".bmp" };
}

void dap::TextureImporter::Import ( std::string key, std::string path ) const {
	if ( dap::TextureAtlas* atlas = dap::TextureAtlas::GetMainAtlas () ) {
		std::cout << "Importing texture: " + key << std::endl;
		atlas->AddImage ( path );
	}
}
