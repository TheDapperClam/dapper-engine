#pragma once

#include "ImGuiWindow.h"
#include <string>

namespace dap {
	class Tileset;

	class ImGuiTilesetEditor : public dap::ImGuiWindow {
	private:
		dap::Tileset* targetTileset = 0;

	public:
		ImGuiTilesetEditor ( std::string title );

	public:
		void SetTargetTileset ( dap::Tileset* target );

	protected:
		virtual void ContentBegin () override;
	};
};
