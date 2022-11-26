#pragma once

#include "ImGuiWindow.h"
#include <string>

namespace dap {
	class Tilemap;

	class ImGuiTilemapEditor : public dap::ImGuiWindow {
	public:
		enum Mode {
			Draw = 0,
			Erase
		};

	private:
		static dap::ImGuiTilemapEditor* Current;

	public:
		static dap::ImGuiTilemapEditor* const GetCurrent ();

	private:
		dap::Tilemap* targetTilemap = NULL;
		dap::ImGuiTilemapEditor::Mode mode = dap::ImGuiTilemapEditor::Mode::Draw;
		unsigned int selectedTileIndex = 0;

	public:
		ImGuiTilemapEditor ( std::string title );
		~ImGuiTilemapEditor ();

	public:
		void SetTilemap ( dap::Tilemap* map );

	public:
		virtual void ContentBegin () override;
	};
}
