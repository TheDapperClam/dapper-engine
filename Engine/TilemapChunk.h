#pragma once

#include "glm/glm.hpp"
#include "RenderObject.h"

#define TILEMAP_CHUNK_SIZE 16

namespace dap {
	class TilemapChunk : public dap::RenderObject {
	private:
		unsigned int tiles[TILEMAP_CHUNK_SIZE][TILEMAP_CHUNK_SIZE];
		unsigned short nonEmptyTiles = 0;
		glm::vec2 offset;

	public:
		TilemapChunk ( glm::vec2 offset );
	
	public:
		void Clear ( unsigned int value = 0 );
		bool Empty ();
		unsigned int GetTile ( int x, int y );
		void SetTile ( int x, int y, unsigned int value );
		void SetTile ( glm::ivec2 point, unsigned int value );

	protected:
		virtual void BuildElements () override;
		virtual void BuildVertices () override;
	};
}
