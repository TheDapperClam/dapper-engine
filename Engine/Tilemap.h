#pragma once

#include "Component.h"
#include "glm/glm.hpp"
#include "glm/gtx/hash.hpp"
#include <unordered_map>

namespace dap {
	class TilemapChunk;
	class Tileset;

	class Tilemap : public dap::Component {
	private:
		std::unordered_map<glm::ivec2, dap::TilemapChunk*> chunks;
		dap::Tileset* tileset;

	public:
		virtual ~Tilemap () override;

	public:
		dap::Tileset* const GetTileset ();
		glm::ivec2 PointToChunk ( glm::vec2 point );
		glm::vec2 PointToMap ( glm::vec2 point );
		glm::ivec2 PointToTile ( glm::vec2 point );
		void SetTile ( glm::vec2 point, unsigned int value );
		void SetTile ( int x, int y, unsigned int value );
		void SetTileset ( dap::Tileset* set );

	private:
		void DeleteChunk ( glm::ivec2 point );

	public:
		virtual void ImGuiWidgetsBegin () override;
	};
}
