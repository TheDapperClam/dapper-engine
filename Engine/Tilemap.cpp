#include "Tilemap.h"

#include <GameObject.h>
#include <imgui.h>
#include <TilemapChunk.h>
#include <ImGuiTilemapEditor.h>
#include <Tileset.h>
#include <Transform.h>

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

std::vector<glm::vec2> dap::Tilemap::GetNavWorldPath ( glm::vec2 start, glm::vec2 end, bool allowDiagonal ) {
	std::vector<glm::vec2> path;

	const glm::vec2 START_MAP_POINT = glm::floor ( PointWorldToMap ( start ) );
	const glm::vec2 END_MAP_POINT = glm::floor ( PointWorldToMap ( end ) );
	if ( !GetTileWorld ( PointWorldToTile ( start ) ) || !GetTileWorld ( PointWorldToTile ( end ) ) )
		return path;

	// Initially set our current node to the starting one
	dap::Tilemap::NavNode startingNode = dap::Tilemap::NavNode ( START_MAP_POINT, 0.0f, glm::distance ( START_MAP_POINT, END_MAP_POINT ), NULL );
	dap::Tilemap::NavNode* nodeToCheck = &startingNode;
	std::unordered_map<glm::vec2, dap::Tilemap::NavNode*> allNavNodes;
	std::vector<dap::Tilemap::NavNode*> uncheckedNavNodes;

	const float ORTHOGONAL_DIST = 1.0f;
	const glm::vec2 PATH_FINAL_OFFSET = glm::vec2 ( ORTHOGONAL_DIST / 2.0f, ORTHOGONAL_DIST / 2.0f );
	bool endReached = false;
	do {
		for ( float y = -ORTHOGONAL_DIST; y <= ORTHOGONAL_DIST; y += ORTHOGONAL_DIST ) {
			for ( float x = -ORTHOGONAL_DIST; x <= ORTHOGONAL_DIST; x += ORTHOGONAL_DIST ) {
				const glm::vec2 NEW_NODE_POS = nodeToCheck->Point + glm::vec2 ( x, y );
				if ( x == 0 && y == 0 || ( !allowDiagonal && y != 0 && x != 0 ) )
					continue;
				if ( !GetTileWorld ( NEW_NODE_POS ) )
					continue;
				if ( NEW_NODE_POS == END_MAP_POINT ) {
					endReached = true;
					path.push_back ( NEW_NODE_POS + PATH_FINAL_OFFSET );
					dap::Tilemap::NavNode* currentFinalPathNode = nodeToCheck;
					while ( currentFinalPathNode->ParentNode != NULL ) {
						path.push_back ( currentFinalPathNode->Point + PATH_FINAL_OFFSET );
						currentFinalPathNode = currentFinalPathNode->ParentNode;
					}
					std::reverse ( path.begin (), path.end () );
					break;
				}
				if ( allNavNodes.contains ( NEW_NODE_POS ) ) {
					// Change the existing node's parent to the one with the shortest path from the start
					if ( nodeToCheck->StartDistance < allNavNodes[NEW_NODE_POS]->ParentNode->StartDistance )
						allNavNodes[NEW_NODE_POS]->ParentNode = nodeToCheck;
					continue;
				}
				dap::Tilemap::NavNode* newNode = new dap::Tilemap::NavNode (
					NEW_NODE_POS,
					glm::distance ( START_MAP_POINT, NEW_NODE_POS ),
					glm::distance ( END_MAP_POINT, NEW_NODE_POS ),
					nodeToCheck
				);
				allNavNodes.insert ( std::pair<glm::vec2, dap::Tilemap::NavNode*> ( NEW_NODE_POS, newNode ) );
				uncheckedNavNodes.push_back ( newNode );
			}
			if ( endReached )
				break;
		}

		if ( !endReached ) {
			const int UNCHECKED_START_INDEX = uncheckedNavNodes.size () - 1;
			int eraseNodeIndex = UNCHECKED_START_INDEX;
			for ( int i = UNCHECKED_START_INDEX; i >= 0; i-- ) {
				dap::Tilemap::NavNode* currentUncheckedNode = uncheckedNavNodes[i];
				if ( i == UNCHECKED_START_INDEX ) {
					nodeToCheck = currentUncheckedNode;
					continue;
				}
				if ( currentUncheckedNode->SumDistance < nodeToCheck->SumDistance || ( currentUncheckedNode->SumDistance == nodeToCheck->SumDistance && currentUncheckedNode->EndDistance < nodeToCheck->EndDistance ) ) {
					nodeToCheck = currentUncheckedNode;
					eraseNodeIndex = i;
				}
			}
			uncheckedNavNodes.erase ( uncheckedNavNodes.begin () + eraseNodeIndex );
		}
	} while ( !endReached && uncheckedNavNodes.size () > 0 );

	for ( const auto& NODE_PAIR : allNavNodes )
		delete NODE_PAIR.second;

	std::cout << "[TILEMAP] Navigation path generated with " << path.size () << " points" << std::endl;
	return path;
}

unsigned int dap::Tilemap::GetTileWorld ( glm::vec2 point ) {
	const glm::ivec2 TILE_POINT = PointWorldToTile ( point );
	const glm::ivec2 CHUNK_POINT = PointWorldToChunk ( point );
	if ( !chunks.contains ( CHUNK_POINT ) )
		return 0;
	return chunks[CHUNK_POINT]->GetTile ( TILE_POINT.x, TILE_POINT.y );
}

unsigned int dap::Tilemap::GetTileWorld ( float x, float y ) {
	return GetTileWorld ( glm::vec2 ( x, y ) );
}

dap::Tileset* const dap::Tilemap::GetTileset () {
	return tileset;
}

void dap::Tilemap::ImGuiWidgetsBegin () {
	dap::Component::ImGuiWidgetsBegin ();

	if ( ImGui::Button ( "Edit" ) ) {
		dap::ImGuiTilemapEditor* editor = dap::ImGuiTilemapEditor::GetCurrent ();
		if ( editor ) {
			editor->SetTilemap ( this );
			editor->SetShow ( true );
		}
	}

	if ( ImGui::BeginCombo ( "Tileset", tileset ? tileset->GetName ().c_str () : "" ) ) {
		for ( const auto& TILESET_PAIR : *dap::Tileset::GetAllTilesets () ) {
			if ( ImGui::Selectable ( TILESET_PAIR.first.c_str () ) )
				SetTileset ( TILESET_PAIR.second );
		}
		ImGui::EndCombo ();
	}
}

glm::ivec2 dap::Tilemap::PointWorldToChunk ( glm::vec2 point ) {
	const glm::vec2 RELATIVE_POSITION = PointWorldToMap ( point );
	const glm::vec2 CHUNK_POSITION = glm::floor ( glm::vec2 ( RELATIVE_POSITION.x / TILEMAP_CHUNK_SIZE, RELATIVE_POSITION.y / TILEMAP_CHUNK_SIZE ) );
	return CHUNK_POSITION;
}

glm::vec2 dap::Tilemap::PointWorldToMap ( glm::vec2 point ) {
	const glm::vec2 MAP_POSITION = GetOwner ()->GetTransform ()->GetPosition ();
	const glm::vec2 RELATIVE_POSITION = point - MAP_POSITION;
	return RELATIVE_POSITION;
}

glm::ivec2 dap::Tilemap::PointWorldToTile ( glm::vec2 point ) {
	const glm::vec2 RELATIVE_POSITION = PointWorldToMap ( point );
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
	const glm::ivec2 CHUNK_POSITION = PointWorldToChunk ( point );
	const glm::ivec2 TILE_POSITION = PointWorldToTile ( point );

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

void dap::Tilemap::SetTileWorld ( float x, float y, unsigned int value ) {
	SetTileWorld ( glm::vec2 ( x, y ), value );
}

void dap::Tilemap::SetTileset ( dap::Tileset* set ) {
	tileset = set;
	for ( const auto& CHUNK_PAIR : chunks ) {
		CHUNK_PAIR.second->SetTileset ( set );
	}
}
