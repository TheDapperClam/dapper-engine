#include "TextureImporter.h"
#include <iostream>
#include "Resources.h"
#include "Texture.h"

std::vector<std::string> dap::TextureImporter::GetExtensions () const {
	return { ".jpg", ".png" };
}

void dap::TextureImporter::Import ( std::string key, std::string path ) const {
	dap::Resources::Add ( key, new dap::Texture ( path, key ) );
}
