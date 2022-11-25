#include "Tilemap.h"

#include "GameObject.h"
#include "imgui.h"
#include "TilemapChunk.h"
#include "Tileset.h"
#include "Transform.h"

dap::Tilemap::~Tilemap () {
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

void dap::Tilemap::SetTile ( glm::vec2 point, unsigned int value ) {
	const glm::ivec2 CHUNK_POSITION = PointToChunk ( point );
	const glm::ivec2 TILE_POSITION = PointToTile ( point );

	if ( value > 0 && !chunks.contains ( CHUNK_POSITION ) ) {
		dap::TilemapChunk* newChunk = new dap::TilemapChunk ( CHUNK_POSITION * TILEMAP_CHUNK_SIZE, this );
		chunks.insert ( std::pair<glm::ivec2, dap::TilemapChunk*> ( CHUNK_POSITION, newChunk ) );
		newChunk->SetTile ( TILE_POSITION, value );
	}
	if ( chunks.contains ( CHUNK_POSITION ) ) {
		chunks[CHUNK_POSITION]->SetTile ( TILE_POSITION, value );
		if ( chunks[CHUNK_POSITION]->Empty () )
			DeleteChunk ( CHUNK_POSITION );
	}
}

void dap::Tilemap::SetTile ( int x, int y, unsigned int value ) {
	SetTile ( glm::ivec2 ( x, y ), value );
}

void dap::Tilemap::SetTileset ( dap::Tileset* set ) {
	tileset = set;
	for ( const auto& CHUNK_PAIR : chunks ) {
		CHUNK_PAIR.second->SetTileset ( set );
	}
}
