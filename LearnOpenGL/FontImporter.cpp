#include "FontImporter.h"
#include "Resources.h"
#include "Font.h"

std::vector<std::string> dap::FontImporter::GetExtensions () const {
	return {".ttf"};
}

void dap::FontImporter::Import ( std::string key, std::string path ) const {
	// TODO: create font resource object
	dap::Font* newFont = new dap::Font ( path );
	dap::Resources::Add ( key, newFont );
}
