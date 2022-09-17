#pragma once

#include "Component.h"
#include "glm/glm.hpp"
#include "glm/gtx/hash.hpp"
#include <unordered_map>

namespace dap {
	class TilemapChunk;

	class Tilemap : public dap::Component {
	private:
		std::unordered_map<glm::ivec2, dap::TilemapChunk*> chunks;

	public:
		virtual ~Tilemap () override;

	public:
		glm::ivec2 PointToChunk ( glm::vec2 point );
		glm::vec2 PointToMap ( glm::vec2 point );
		glm::ivec2 PointToTile ( glm::vec2 point );
		void SetTile ( glm::vec2 point, unsigned int value );
		void SetTile ( int x, int y, unsigned int value );

	private:
		void DeleteChunk ( glm::ivec2 point, bool removeFromMap );
	};
}
