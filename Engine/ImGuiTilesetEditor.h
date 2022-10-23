#pragma once

#include "ImGuiFileBrowser.h"
#include "ImGuiWindow.h"
#include <map>
#include <string>
#include "Tileset.h"

namespace dap {
	struct TextureAtlasSubTexture;

	class ImGuiTilesetEditor : public dap::ImGuiWindow {
	private:
		static const float TILE_LIST_ELEM_SIZE;
		static const std::map<std::string, dap::Tileset*>* ALL_TILESETS;

	private:
		dap::ImGuiFileBrowser* fileBrowser = 0;
		dap::TextureAtlasSubTexture* selectedTile = 0;
		dap::Tileset targetTileset = dap::Tileset ( "", false );

	public:
		ImGuiTilesetEditor ( std::string title );
		~ImGuiTilesetEditor ();

	private:
		void OnFileBrowserRespond ( dap::ImGuiFileBrowser::Response response, std::string& path );

	protected:
		virtual void ContentBegin () override;
	};
};
