#include "FontImporter.h"
#include "Resources.h"
#include "Font.h"

std::vector<std::string> dap::FontImporter::GetExtensions () {
	return {".ttf"};
}

void dap::FontImporter::Import ( std::string key, std::string path ) {
	dap::Font* newFont = new dap::Font ( path );
	fontsToFree.push_back ( newFont );
	dap::Resources::Add ( key, newFont );
}

void dap::FontImporter::ImportingComplete () {
	for ( const auto& font : fontsToFree ) {
		font->Free ();
	}
}
