#include "ImGuiTilesetEditor.h"

dap::ImGuiTilesetEditor::ImGuiTilesetEditor ( std::string title ) : dap::ImGuiWindow::ImGuiWindow ( title ) {

}

void dap::ImGuiTilesetEditor::ContentBegin () {
	bool checked = true;
	ImGui::Checkbox ( "Hello", &checked );
}

void dap::ImGuiTilesetEditor::SetTargetTileset ( dap::Tileset* target ) {
	targetTileset = target;
}
