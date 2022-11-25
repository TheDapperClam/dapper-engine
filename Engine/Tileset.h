#pragma once

#include <glm/glm.hpp>
#include "JSONObject.h"
#include <map>
#include "nlohmann/json.hpp"
#include <string>
#include <unordered_map>
#include <vector>

namespace dap {
	struct TextureAtlasSubTexture;

	class Tileset : public dap::JSONObject {
		FUNC_GET_FILE_EXTENSION (".dts")

	private:
		static std::map<std::string, dap::Tileset*> AllTilesets;
		static bool ClearingAllTilesets;

	public:
		static void ClearAllTilesets ();
		static const std::map<std::string, dap::Tileset*>* GetAllTilesets ();

	private:
		std::vector<dap::TextureAtlasSubTexture*> linearTiles = {};
		std::string name = "TileSet";
		dap::TextureAtlasSubTexture* primaryTexture = NULL;
		glm::ivec2 tileSize = glm::ivec2 ( 64, 64 );

	public:
		Tileset ( std::string name, bool appendToList = true );
		~Tileset ();

	public:
		void ClearTiles ();
		void CreateTile ( int x = 0, int y = 0 );
		void DeleteTile ( dap::TextureAtlasSubTexture* tile );
		const std::vector<dap::TextureAtlasSubTexture*>* GetAllTiles ();
		std::string GetName ();
		const dap::TextureAtlasSubTexture* GetPrimaryTexture ();
		const dap::TextureAtlasSubTexture* GetTile ( unsigned int index );
		glm::ivec2 GetTileSize ();
		void SetPrimaryTexture ( dap::TextureAtlasSubTexture* texture );
		void SetTileSize ( unsigned int width, unsigned int height );
		void SetTileSize ( glm::ivec2 size );

	public:
		virtual void Deserialize ( const nlohmann::json* json ) override;
		virtual nlohmann::json Serialize () override;
	};
}
