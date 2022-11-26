#include "Tilemap.h"

#include "GameObject.h"
#include "imgui.h"
#include "TilemapChunk.h"
#include "Tileset.h"
#include "Transform.h"
#include <vector>

dap::Tilemap::~Tilemap () {
	ClearChunks ();
}

void dap::Tilemap::AddChunk ( glm::ivec2 position, dap::TilemapChunk* chunk ) {
	chunks.insert ( std::pair<glm::ivec2, dap::TilemapChunk*> ( position, chunk ) );
}

void dap::Tilemap::ClearChunks () {
	for ( const auto& CHUNK_PAIR : chunks ) {
		delete CHUNK_PAIR.second;
	}
	chunks.clear ();
}

void dap::Tilemap::DeleteChunk ( glm::ivec2 point ) {
	if ( !chunks.contains ( point ) )
		return;
	delete chunks.at ( point );
	chunks.erase ( point );
}

void dap::Tilemap::Deserialize ( const nlohmann::json* json ) {
	dap::Component::Deserialize ( json );

	ClearChunks ();

	if ( json->contains ( "tileset" ) )
		SetTileset ( dap::Tileset::GetTileset ( json->at ( "tileset" ) ) );

	if ( json->contains ( "chunks" ) ) {
		for ( const auto& CHUNK_JSON : json->at ( "chunks" ) ) {
			dap::TilemapChunk* chunk = new dap::TilemapChunk ( glm::vec2 ( 0, 0 ), this );
			chunk->Deserialize ( &CHUNK_JSON );
			AddChunk ( glm::ivec2 ( CHUNK_JSON.at("mapPosition")[0], CHUNK_JSON.at("mapPosition")[1] ), chunk );
		}
	}
}

dap::Tileset* const dap::Tilemap::GetTileset () {
	return tileset;
}

void dap::Tilemap::ImGuiWidgetsBegin () {
	dap::Component::ImGuiWidgetsBegin ();

	if ( ImGui::BeginCombo ( "Tileset", tileset ? tileset->GetName ().c_str () : "" ) ) {
		for ( const auto& TILESET_PAIR : *dap::Tileset::GetAllTilesets () ) {
			if ( ImGui::Selectable ( TILESET_PAIR.first.c_str () ) )
				SetTileset ( TILESET_PAIR.second );
		}
		ImGui::EndCombo ();
	}
}

glm::ivec2 dap::Tilemap::PointToChunk ( glm::vec2 point ) {
	const glm::vec2 RELATIVE_POSITION = PointToMap ( point );
	const glm::vec2 CHUNK_POSITION = glm::floor ( glm::vec2 ( RELATIVE_POSITION.x / TILEMAP_CHUNK_SIZE, RELATIVE_POSITION.y / TILEMAP_CHUNK_SIZE ) );
	return CHUNK_POSITION;
}

glm::vec2 dap::Tilemap::PointToMap ( glm::vec2 point ) {
	const glm::vec2 MAP_POSITION = GetOwner ()->GetTransform ()->GetPosition ();
	const glm::vec2 RELATIVE_POSITION = point - MAP_POSITION;
	return RELATIVE_POSITION;
}

glm::ivec2 dap::Tilemap::PointToTile ( glm::vec2 point ) {
	const glm::vec2 RELATIVE_POSITION = PointToMap ( point );
	const glm::ivec2 UNCLAMPED_TILE_POSITION = glm::floor ( RELATIVE_POSITION );
	glm::ivec2 tilePosition = glm::ivec2 ( UNCLAMPED_TILE_POSITION.x % TILEMAP_CHUNK_SIZE, UNCLAMPED_TILE_POSITION.y % TILEMAP_CHUNK_SIZE );
	if ( tilePosition.x < 0.0f )
		tilePosition.x += TILEMAP_CHUNK_SIZE;
	if ( tilePosition.y < 0.0f )
		tilePosition.y += TILEMAP_CHUNK_SIZE;
	return tilePosition;
}

nlohmann::json dap::Tilemap::Serialize () {
	nlohmann::json json = dap::Component::Serialize ();
	json["tileset"] = tileset ? tileset->GetName () : "";

	std::vector<nlohmann::json> chunksJson;
	chunksJson.reserve ( chunks.size () );
	for ( const auto& CHUNK_PAIR : chunks ) {
		nlohmann::json chunk = CHUNK_PAIR.second->Serialize ();
		chunk["mapPosition"] = { CHUNK_PAIR.first.x, CHUNK_PAIR.first.y };
		chunksJson.push_back ( chunk );
	}
	json["chunks"] = chunksJson;

	return json;
}

#include "glm/gtx/string_cast.hpp"

void dap::Tilemap::SetTileWorld ( glm::vec2 point, unsigned int value ) {
	const glm::ivec2 CHUNK_POSITION = PointToChunk ( point );
	const glm::ivec2 TILE_POSITION = PointToTile ( point );

	if ( !chunks.contains ( CHUNK_POSITION ) ) {
		if ( value > 0 ) {
			dap::TilemapChunk* newChunk = new dap::TilemapChunk ( CHUNK_POSITION * TILEMAP_CHUNK_SIZE, this );
			AddChunk ( CHUNK_POSITION, newChunk );
			newChunk->SetTile ( TILE_POSITION, value );
		}
	} else {
		chunks[CHUNK_POSITION]->SetTile ( TILE_POSITION, value );
		if ( chunks[CHUNK_POSITION]->Empty () )
			DeleteChunk ( CHUNK_POSITION );
	}
}

void dap::Tilemap::SetTileWorld ( int x, int y, unsigned int value ) {
	SetTileWorld ( glm::ivec2 ( x, y ), value );
}

void dap::Tilemap::SetTileset ( dap::Tileset* set ) {
	tileset = set;
	for ( const auto& CHUNK_PAIR : chunks ) {
		CHUNK_PAIR.second->SetTileset ( set );
	}
}
