#pragma once

#include "glm/glm.hpp"
#include "RenderObject.h"

#define TILEMAP_CHUNK_SIZE 16

namespace dap {
	class Tilemap;
	class Tileset;

	class TilemapChunk : public dap::RenderObject {
	private:
		unsigned short nonEmptyTiles = 0;
		glm::vec2 offset;
		dap::Tilemap* tilemap = NULL;
		unsigned int tiles[TILEMAP_CHUNK_SIZE][TILEMAP_CHUNK_SIZE];
		dap::Tileset* tileset = NULL;

	public:
		TilemapChunk ( glm::vec2 offset, dap::Tilemap* owner );
	
	public:
		void Clear ( unsigned int value = 0 );
		bool Empty ();
		unsigned int GetTile ( int x, int y );
		void SetTile ( int x, int y, unsigned int value, bool updateMesh = true );
		void SetTile ( glm::ivec2 point, unsigned int value, bool updateMesh = true );
		void SetTileset ( dap::Tileset* set );

	protected:
		virtual void BuildElements () override;
		virtual void BuildVertices () override;
	};
}
