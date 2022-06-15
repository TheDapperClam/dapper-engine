#include "TextureImporter.h"
#include "Rendering.h"
#include <iostream>

std::vector<std::string> dap::TextureImporter::GetExtensions () const {
	return { ".jpg", ".png" };
}

void dap::TextureImporter::Import ( std::string key, std::string path ) const {
	dap::Rendering::LoadTexture ( key, path );
}
