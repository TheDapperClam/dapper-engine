#include "Tileset.h"

#include "TextureAtlas.h"
#include "TextureAtlasSubTexture.h"

std::map<std::string, dap::Tileset*> dap::Tileset::AllTilesets = {};

dap::Tileset::Tileset ( std::string name ) {
	this->name = name;
	AllTilesets.insert ( std::pair<std::string, dap::Tileset*> ( name, this ) );
}

dap::Tileset::~Tileset () {
	for ( const auto& TILE : linearTiles ) {
		delete TILE;
	}
	linearTiles.clear ();
	AllTilesets.erase ( name );
}

void dap::Tileset::ClearAllTilesets () {
	for ( const auto& TILESET_PAIR : AllTilesets ) {
		delete TILESET_PAIR.second;
	}
	AllTilesets.clear ();
}

void dap::Tileset::CreateTile ( int x, int y ) {
	dap::TextureAtlasSubTexture* newTile = new dap::TextureAtlasSubTexture ( "", x, y, 0.0f, 0.0f, tileSize.x, tileSize.y, 0.0f, 0.0f );
	newTile->MapToSubTexture ( primaryTexture );
	linearTiles.push_back ( newTile );
}

void dap::Tileset::DeleteTile ( dap::TextureAtlasSubTexture* tile ) {
	linearTiles.erase ( std::remove ( linearTiles.begin (), linearTiles.end (), tile ), linearTiles.end () );
	delete tile;
}

void dap::Tileset::Deserialize ( const nlohmann::json* json ) {
	if ( json->contains ( "primaryTexture" ) )
		SetPrimaryTexture ( dap::TextureAtlas::GetMainAtlas ()->GetSubTexture ( json->at ( "primaryTexture" ) ) );
	if ( json->contains ( "tileSize" ) ) {
		const nlohmann::json SIZE = json->at ( "tileSize" );
		SetTileSize ( SIZE[0], SIZE[1] );
	}
	if ( json->contains ( "tiles" ) ) {
		for ( const auto& TILE_JSON : json->at ( "tiles" ) ) {
			CreateTile ( TILE_JSON.at ( "x" ), TILE_JSON.at ( "y" ) );
		}
	}
}

const std::vector<dap::TextureAtlasSubTexture*>* dap::Tileset::GetAllTiles () {
	return &linearTiles;
}

const dap::TextureAtlasSubTexture* dap::Tileset::GetPrimaryTexture () {
	return primaryTexture;
}

const dap::TextureAtlasSubTexture* dap::Tileset::GetTile ( unsigned int index ) {
	if ( index < 0 || index >= linearTiles.size () )
		return NULL;
	return linearTiles[index];
}

glm::ivec2 dap::Tileset::GetTileSize () {
	return tileSize;
}

nlohmann::json dap::Tileset::Serialize () {
	nlohmann::json json = dap::JSONObject::Serialize ();
	json["primaryTexture"] = primaryTexture ? primaryTexture->Name : "";
	json["tileSize"] = { tileSize.x, tileSize.y };
	std::vector<nlohmann::json> tilesJson;
	tilesJson.reserve ( linearTiles.size () );
	for ( const auto& TILE : linearTiles )
		tilesJson.push_back ( TILE->Serialize () );
	json["tiles"] = tilesJson;
	return json;
}

void dap::Tileset::SetPrimaryTexture ( dap::TextureAtlasSubTexture* texture ) {
	primaryTexture = texture;
	for ( int i = 0; i < linearTiles.size (); i++ ) {
		dap::TextureAtlasSubTexture* tile = linearTiles[i];
		tile->MapToSubTexture ( primaryTexture );
	}
}

void dap::Tileset::SetTileSize ( unsigned int width, unsigned int height ) {
	tileSize = glm::ivec2 ( width, height );
	for ( int i = 0; i < linearTiles.size (); i++ ) {
		dap::TextureAtlasSubTexture* tile = linearTiles[i];
		tile->ImageWidth = width;
		tile->ImageHeight = height;
		tile->MapToSubTexture ( primaryTexture );
	}
}

void dap::Tileset::SetTileSize ( glm::ivec2 size ) {
	SetTileSize ( size.x, size.y );
}
