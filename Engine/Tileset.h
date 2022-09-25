#pragma once

#include <glm/glm.hpp>
#include "JSONObject.h"
#include <map>
#include "nlohmann/json.hpp"
#include <unordered_map>
#include <vector>

namespace dap {
	struct TextureAtlasSubTexture;

	class Tileset : public dap::JSONObject {
	private:
		static std::map<std::string, dap::Tileset*> AllTilesets;

	private:
		std::vector<dap::TextureAtlasSubTexture*> linearTiles = {};
		std::string name = "TileSet";
		dap::TextureAtlasSubTexture* primaryTexture = NULL;
		glm::ivec2 tileSize = glm::ivec2 ( 1, 1 );

	public:
		Tileset ( std::string name );
		~Tileset ();

	public:
		void CreateTile ( int x = 0, int y = 0 );
		const std::vector<dap::TextureAtlasSubTexture*>* GetAllTiles ();
		const dap::TextureAtlasSubTexture* GetTile ( unsigned int index );
		void SetPrimaryTexture ( dap::TextureAtlasSubTexture* texture );
		void SetTileSize ( unsigned int width, unsigned int height );
		void SetTileSize ( glm::ivec2 size );

	public:
		virtual void Deserialize ( const nlohmann::json* json ) override;
		virtual nlohmann::json Serialize () override;
	};
}
