#pragma once

#include "ImGuiWindow.h"
#include <map>
#include <string>

namespace dap {
	struct TextureAtlasSubTexture;
	class Tileset;

	class ImGuiTilesetEditor : public dap::ImGuiWindow {
	private:
		static const float TILE_LIST_ELEM_SIZE;
		static const std::map<std::string, dap::Tileset*>* ALL_TILESETS;

	private:
		bool isNewTilesetPopupOpen = false;
		std::string newTilesetName = "";
		dap::TextureAtlasSubTexture* selectedTile = 0;
		dap::Tileset* targetTileset = 0;

	public:
		ImGuiTilesetEditor ( std::string title );

	public:
		void SetTargetTileset ( dap::Tileset* target );

	private:
		void ShowNewTilesetPopup ();

	protected:
		virtual void ContentBegin () override;
	};
};
