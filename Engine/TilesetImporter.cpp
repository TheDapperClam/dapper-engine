#include "TilesetImporter.h"

#include "nlohmann/json.hpp"
#include "Tileset.h"

std::unordered_map<dap::Tileset*, std::string> dap::TilesetImporter::TilesetPaths = {};

void dap::TilesetImporter::ClearImports () {
	dap::Tileset::ClearAllTilesets ();
}

void dap::TilesetImporter::Import ( std::string key, std::string path ) {
	dap::Tileset* ts = new dap::Tileset ( key );
	TilesetPaths.insert ( std::pair<dap::Tileset*, std::string> ( ts, path ) );
}

void dap::TilesetImporter::TextureAtlasBuilt () {
	for ( const auto& PATH_PAIR : TilesetPaths ) {
		PATH_PAIR.first->LoadFile ( PATH_PAIR.second );
	}
	TilesetPaths.clear ();
}

std::vector<std::string> dap::TilesetImporter::GetExtensions () {
	return { dap::Tileset::GetFileExtension () };
}
