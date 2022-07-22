#include "FontImporter.h"
#include "Resources.h"

std::vector<std::string> dap::FontImporter::GetExtensions () const {
	return {".ttf"};
}

void dap::FontImporter::Import ( std::string key, std::string path ) const {
	// TODO: create font resource object
	dap::Resources::Add ( key, NULL );
}
