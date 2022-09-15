#pragma once

#include "RenderObject.h"

#define TILEMAP_CHUNK_SIZE 16

namespace dap {
	class TilemapChunk : public dap::RenderObject {
	private:
		unsigned int tiles[TILEMAP_CHUNK_SIZE][TILEMAP_CHUNK_SIZE];

	public:
		TilemapChunk ();
	
	public:
		void Clear ( unsigned int value = 0 );
		unsigned int GetTile ( int x, int y );
		void SetTile ( int x, int y, unsigned int value );

	protected:
		virtual void BuildElements () override;
		virtual void BuildVertices () override;
	};
}
