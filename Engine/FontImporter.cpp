#include "FontImporter.h"

#include <iostream>
#include "Font.h"

void dap::FontImporter::ClearImports () {
	dap::Font::ClearFonts ();
}

std::vector<std::string> dap::FontImporter::GetExtensions () {
	return {".ttf"};
}

void dap::FontImporter::Import ( std::string key, std::string path ) {
	std::cout << "[IMPORTER::FONT] Importing font: " << key << std::endl;
	dap::Font* newFont = new dap::Font ( key, path );
	fontsToFree.push_back ( newFont );
}

void dap::FontImporter::ImportingComplete () {
	for ( const auto& font : fontsToFree ) {
		font->Free ();
	}
}
