#include "TilemapChunk.h"

#include "glm/glm.hpp"
#include <math.h>
#include "Shader.h"
#include "Tilemap.h"
#include "Tileset.h"
#include <vector>
#include "Vertex.h"

#include "glm/gtx/string_cast.hpp"
#include "TextureAtlas.h"
#include "TextureAtlasSubTexture.h"

dap::TilemapChunk::TilemapChunk ( glm::vec2 offset, dap::Tilemap* tilemap ) : dap::RenderObject::RenderObject ( false ) {
	this->offset = offset;
	this->tilemap = tilemap;
	if ( tilemap ) {
		SetTileset ( tilemap->GetTileset () );
		SetOwner ( tilemap->GetOwner () );
	}
	SetShader ( dap::Shader::DEFAULT_SPRITE );
	Clear ();
}

void dap::TilemapChunk::BuildElements () {
	if ( !tileset )
		return;
	unsigned int elementOffset = 0;

	elements.clear ();
	for ( int y = 0; y < TILEMAP_CHUNK_SIZE; y++ ) {
		for ( int x = 0; x < TILEMAP_CHUNK_SIZE; x++ ) {
			const unsigned int TILE_INDEX = GetTile ( x, y );
			if ( !TILE_INDEX )
				continue;
			if ( !tileset->GetTile ( TILE_INDEX - 1 ) )
				continue;
			elements.insert ( elements.end (), {
				elementOffset,
				elementOffset + 1,
				elementOffset + 2,
				elementOffset + 2,
				elementOffset + 3,
				elementOffset
			} );
			elementOffset += 4;
		}
	}
}

void dap::TilemapChunk::BuildVertices () {
	if ( !tileset )
		return;
	vertices.clear ();
	for ( int y = 0; y < TILEMAP_CHUNK_SIZE; y++ ) {
		for ( int x = 0; x < TILEMAP_CHUNK_SIZE; x++ ) {
			const unsigned int TILE_INDEX = GetTile ( x, y );
			if ( !TILE_INDEX )
				continue;
			const dap::TextureAtlasSubTexture* SUB_TEX = tileset->GetTile ( TILE_INDEX - 1 );
			if ( !SUB_TEX )
				continue;
			const auto TEX_COORDS = SUB_TEX->GetTexCoords ();
			vertices.insert ( vertices.end (), {
				dap::Vertex ( glm::vec3 ( x + offset.x, 1.0f + y + offset.y, 0.0f ), glm::vec3 ( 1.0f, 0.0f, 0.0f ), TEX_COORDS[0] ),
				dap::Vertex ( glm::vec3 ( 1.0f + x + offset.x, 1.0f + y + offset.y, 0.0f ), glm::vec3 ( 0.0f, 1.0f, 0.0f ), TEX_COORDS[1] ),
				dap::Vertex ( glm::vec3 ( 1.0f + x + offset.x, y + offset.y, 0.0f ), glm::vec3 ( 0.0f, 0.0f, 1.0f ), TEX_COORDS[2] ),
				dap::Vertex ( glm::vec3 ( x + offset.x, y + offset.y, 0.0f ), glm::vec3 ( 0.0f, 1.0f, 0.0f ), TEX_COORDS[3] )
			} );
		}
	}
}

void dap::TilemapChunk::Clear ( unsigned int value ) {
	for ( int y = 0; y < TILEMAP_CHUNK_SIZE; y++ ) {
		for ( int x = 0; x < TILEMAP_CHUNK_SIZE; x++ ) {
			SetTile ( x, y, 0, false );
		}
	}
	BuildVertices ();
	BuildElements ();
	BuildGLObject ();
}

void dap::TilemapChunk::Deserialize ( const nlohmann::json* json ) {
	dap::RenderObject::Deserialize ( json );

	if ( json->contains ( "offset" ) ) {
		float x = json->at ( "offset" )[0];
		float y = json->at ( "offset" )[1];
		offset = glm::vec2 ( x, y );
	}

	if ( json->contains ( "tiles" ) ) {
		const nlohmann::json TILE_LIST = json->at ( "tiles" );

		for ( int y = 0; y < TILEMAP_CHUNK_SIZE; y++ ) {
			for ( int x = 0; x < TILEMAP_CHUNK_SIZE; x++ ) {
				const unsigned int TILE = TILE_LIST[ y * TILEMAP_CHUNK_SIZE + x ];
				SetTile ( x, y, TILE, false );
			}
		}
	}

	BuildElements ();
	BuildVertices ();
	BuildGLObject ();
}

bool dap::TilemapChunk::Empty () {
	return nonEmptyTiles <= 0;
}

glm::ivec2 dap::TilemapChunk::GetOffset () {
	return offset;
}

unsigned int dap::TilemapChunk::GetTile ( int x, int y ) {
	if ( x < 0 || x >= TILEMAP_CHUNK_SIZE || y < 0 || y >= TILEMAP_CHUNK_SIZE )
		return 0;
	return tiles[y][x];
}

nlohmann::json dap::TilemapChunk::Serialize () {
	nlohmann::json json = dap::RenderObject::Serialize ();
	json["offset"] = { offset.x, offset.y };

	std::vector<unsigned int> linearTiles;
	linearTiles.reserve ( TILEMAP_CHUNK_SIZE * TILEMAP_CHUNK_SIZE );
	for ( int y = 0; y < TILEMAP_CHUNK_SIZE; y++ ) {
		for ( int x = 0; x < TILEMAP_CHUNK_SIZE; x++ ) {
			const unsigned int TILE = GetTile ( x, y );
			linearTiles.push_back ( TILE );
		}
	}
	json["tiles"] = linearTiles;

	return json;
}

void dap::TilemapChunk::SetTile ( int x, int y, unsigned int value, bool updateMesh ) {
	if ( x < 0 || x >= TILEMAP_CHUNK_SIZE || y < 0 || y >= TILEMAP_CHUNK_SIZE )
		return;
	if ( GetTile ( x, y ) == value )
		return;
	nonEmptyTiles += value > 0 ? 1 : -1;
	tiles[y][x] = value;

	if ( updateMesh ) {
		BuildElements ();
		BuildVertices ();
		BuildGLObject ();
	}
}

void dap::TilemapChunk::SetTile ( glm::ivec2 point, unsigned int value, bool updateMesh ) {
	SetTile ( point.x, point.y, value, updateMesh );
}

void dap::TilemapChunk::SetTileset ( dap::Tileset* set ) {
	tileset = set;
	BuildElements ();
	BuildVertices ();
	BuildGLObject ();
}
