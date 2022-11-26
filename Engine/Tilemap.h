#pragma once

#include "Component.h"
#include "glm/glm.hpp"
#include "glm/gtx/hash.hpp"
#include "nlohmann/json.hpp"
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
		void SetTileWorld ( glm::vec2 point, unsigned int value );
		void SetTileWorld ( int x, int y, unsigned int value );
		void SetTileset ( dap::Tileset* set );

	private:
		void AddChunk ( glm::ivec2 position, dap::TilemapChunk* chunk );
		void ClearChunks ();
		void DeleteChunk ( glm::ivec2 point );

	public:
		virtual void Deserialize ( const nlohmann::json* json ) override;
		virtual void ImGuiWidgetsBegin () override;
		virtual nlohmann::json Serialize () override;
	};
}
