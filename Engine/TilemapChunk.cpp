#include "TilemapChunk.h"

#include "glm/glm.hpp"
#include "Shader.h"
#include "Vertex.h"

#include "TextureAtlas.h"
#include "TextureAtlasSubTexture.h"

dap::TilemapChunk::TilemapChunk ( glm::vec2 offset ) : dap::RenderObject::RenderObject ( false ) {
	this->offset = offset;
	Clear ();
	SetShader ( dap::Shader::DEFAULT_SPRITE );
	BuildElements ();
	BuildVertices ();
	BuildGLObject ();
}

void dap::TilemapChunk::BuildElements () {
	unsigned int elementOffset = 0;

	elements.clear ();
	for ( int y = 0; y < TILEMAP_CHUNK_SIZE; y++ ) {
		for ( int x = 0; x < TILEMAP_CHUNK_SIZE; x++ ) {
			if ( !GetTile ( x, y ) )
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
	vertices.clear ();
	for ( int y = 0; y < TILEMAP_CHUNK_SIZE; y++ ) {
		for ( int x = 0; x < TILEMAP_CHUNK_SIZE; x++ ) {
			if ( !GetTile ( x, y ) )
				continue;
			const glm::vec2* texCoords = dap::TextureAtlas::GetMainAtlas ()->GetSubTexture ( "Apple" )->GetTexCoords ();
			vertices.insert ( vertices.end (), {
				dap::Vertex ( glm::vec3 ( x + offset.x,  1.0f + y + offset.y, 0.0f ), glm::vec3 ( 1.0f, 0.0f, 0.0f ), texCoords[0] ),
				dap::Vertex ( glm::vec3 ( 1.0f + x + offset.x,  1.0f + y + offset.y, 0.0f ), glm::vec3 ( 0.0f, 1.0f, 0.0f ), texCoords[1] ),
				dap::Vertex ( glm::vec3 ( 1.0f + x + offset.x, y + offset.y, 0.0f ), glm::vec3 ( 0.0f, 0.0f, 1.0f ), texCoords[2] ),
				dap::Vertex ( glm::vec3 ( x + offset.x, y + offset.y, 0.0f ), glm::vec3 ( 0.0f, 1.0f, 0.0f ), texCoords[3] )
			} );
		}
	}
}

void dap::TilemapChunk::Clear ( unsigned int value ) {
	for ( int y = 0; y < TILEMAP_CHUNK_SIZE; y++ ) {
		for ( int x = 0; x < TILEMAP_CHUNK_SIZE; x++ ) {
			SetTile ( x, y, value );
		}
	}
}

bool dap::TilemapChunk::Empty () {
	return nonEmptyTiles <= 0;
}

unsigned int dap::TilemapChunk::GetTile ( int x, int y ) {
	if ( x < 0 || x >= TILEMAP_CHUNK_SIZE || y < 0 || y >= TILEMAP_CHUNK_SIZE )
		return 0;
	return tiles[y][x];
}

void dap::TilemapChunk::SetTile ( int x, int y, unsigned int value ) {
	if ( x < 0 || x >= TILEMAP_CHUNK_SIZE || y < 0 || y >= TILEMAP_CHUNK_SIZE )
		return;
	if ( GetTile ( x, y ) == value )
		return;
	nonEmptyTiles += value > 0 ? 1 : -1;
	tiles[y][x] = value;

	BuildElements ();
	BuildVertices ();
	BuildGLObject ();
}

void dap::TilemapChunk::SetTile ( glm::ivec2 point, unsigned int value ) {
	SetTile ( point.x, point.y, value );
}
