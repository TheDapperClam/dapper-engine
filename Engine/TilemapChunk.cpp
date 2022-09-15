#include "TilemapChunk.h"

#include "glm/glm.hpp"
#include "Shader.h"
#include "Vertex.h"

#include "TextureAtlas.h"
#include "TextureAtlasSubTexture.h"

dap::TilemapChunk::TilemapChunk () : dap::RenderObject::RenderObject ( false ) {
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
				dap::Vertex ( glm::vec3 ( -0.5f + x, 0.5f - y, 0.0f ), glm::vec3 ( 1.0f, 0.0f, 0.0f ), texCoords[0] ),
				dap::Vertex ( glm::vec3 ( 0.5f + x, 0.5f - y, 0.0f ), glm::vec3 ( 0.0f, 1.0f, 0.0f ), texCoords[1] ),
				dap::Vertex ( glm::vec3 ( 0.5f + x, -0.5f - y, 0.0f ), glm::vec3 ( 0.0f, 0.0f, 1.0f ), texCoords[2] ),
				dap::Vertex ( glm::vec3 ( -0.5f + x, -0.5f - y, 0.0f ), glm::vec3 ( 0.0f, 1.0f, 0.0f ), texCoords[3] )
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

unsigned int dap::TilemapChunk::GetTile ( int x, int y ) {
	if ( x < 0 || x >= TILEMAP_CHUNK_SIZE || y < 0 || y >= TILEMAP_CHUNK_SIZE )
		return 0;
	return tiles[y][x];
}

void dap::TilemapChunk::SetTile ( int x, int y, unsigned int value ) {
	if ( x < 0 || x >= TILEMAP_CHUNK_SIZE || y < 0 || y >= TILEMAP_CHUNK_SIZE )
		return;
	tiles[y][x] = value;
}
