#pragma once

#include <Component.h>
#include <glm/glm.hpp>
#include <glm/gtx/hash.hpp>
#include <nlohmann/json.hpp>
#include <unordered_map>
#include <vector>

namespace dap {
	class TilemapChunk;
	class Tileset;

	class Tilemap : public dap::Component {
	public:
		struct NavNode {
			glm::vec2 Point;
			float StartDistance = 0.0f;
			float EndDistance = 0.0f;
			float SumDistance = 0.0f;
			dap::Tilemap::NavNode* ParentNode = NULL;

			NavNode ( glm::vec2 point, float startDistance, float endDistance, dap::Tilemap::NavNode* parentNode ) {
				Point = point;
				StartDistance = startDistance;
				EndDistance = endDistance;
				SumDistance = startDistance + endDistance;
				ParentNode = parentNode;
			}
		};

	public:
		static const unsigned int TILE_EMPTY = 0;

	private:
		std::unordered_map<glm::ivec2, dap::TilemapChunk*> chunks;
		dap::Tileset* tileset;

	public:
		virtual ~Tilemap () override;

	public:
		std::vector<glm::vec2> GetNavWorldPath ( glm::vec2 start, glm::vec2 end, bool allowDiagonal );
		unsigned int GetTileWorld ( glm::vec2 point );
		unsigned int GetTileWorld ( float x, float y );
		dap::Tileset* const GetTileset ();
		glm::ivec2 PointWorldToChunk ( glm::vec2 point );
		glm::vec2 PointWorldToMap ( glm::vec2 point );
		glm::ivec2 PointWorldToTile ( glm::vec2 point );
		void SetTileWorld ( glm::vec2 point, unsigned int value );
		void SetTileWorld ( float x, float y, unsigned int value );
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
