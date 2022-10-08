#pragma once

#include "ImGuiWindow.h"
#include <string>

namespace dap {
	struct TextureAtlasSubTexture;
	class Tileset;

	class ImGuiTilesetEditor : public dap::ImGuiWindow {
	private:
		static const float TILE_LIST_ELEM_SIZE;

	private:
		dap::Tileset* targetTileset = 0;
		dap::TextureAtlasSubTexture* selectedTile = 0;

	public:
		ImGuiTilesetEditor ( std::string title );

	public:
		void SetTargetTileset ( dap::Tileset* target );

	protected:
		virtual void ContentBegin () override;
	};
};
