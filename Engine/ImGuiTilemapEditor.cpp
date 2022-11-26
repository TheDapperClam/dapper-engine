#include "ImGuiTilemapEditor.h"

#include "Camera.h"
#include "GLFW/glfw3.h"
#include "imgui.h"
#include "Input.h"
#include "TextureAtlas.h"
#include "TextureAtlasSubTexture.h"
#include "Tilemap.h"
#include "Tileset.h"

dap::ImGuiTilemapEditor* dap::ImGuiTilemapEditor::Current = NULL;

dap::ImGuiTilemapEditor::ImGuiTilemapEditor ( std::string title) : dap::ImGuiWindow ( title ) {
	advertise = false;
	Current = this;
}

dap::ImGuiTilemapEditor::~ImGuiTilemapEditor () {
	if ( Current == this )
		Current = NULL;
}

dap::ImGuiTilemapEditor* const dap::ImGuiTilemapEditor::GetCurrent () {
	return Current;
}

void dap::ImGuiTilemapEditor::ContentBegin () {
	const ImTextureID TEX_ID = (void*)*dap::TextureAtlas::GetMainAtlas ()->GetTextureObject ();
	dap::Camera* mainCam = dap::Camera::GetCurrent ();

	if ( ImGui::Button ( "Draw" ) ) {
		mode = dap::ImGuiTilemapEditor::Mode::Draw;
	}

	ImGui::SameLine ();
	if ( ImGui::Button ( "Erase" ) ) {
		mode = dap::ImGuiTilemapEditor::Mode::Erase;
	}

	if ( targetTilemap ) {
		
		dap::Tileset* ts = targetTilemap->GetTileset ();

		if ( ts ) {
			ImGui::Separator ();
			unsigned int tileIndex = 1;
			for ( const auto& TILE : *ts->GetAllTiles () ) {
				const auto TEX_COORDS = TILE->GetTexCoords ();
				if ( ImGui::ImageButton ( ( "##tile_" + std::to_string ( tileIndex ) ).c_str (), TEX_ID, ImVec2 ( 64, 64 ), ImVec2 ( TEX_COORDS[0].x, TEX_COORDS[0].y ), ImVec2 ( TEX_COORDS[2].x, TEX_COORDS[2].y ) ) ) {
					selectedTileIndex = tileIndex;
					mode = dap::ImGuiTilemapEditor::Mode::Draw;
				}
				tileIndex++;
			}

			if ( dap::Input::Held ( GLFW_MOUSE_BUTTON_1 ) ) {
				glm::vec2 mouseWorldPos = mainCam->ScreenPointToWorld ( dap::Input::GetMouseScreenPosition () );
				switch ( mode ) {
					case dap::ImGuiTilemapEditor::Mode::Draw:
						targetTilemap->SetTileWorld ( mouseWorldPos, selectedTileIndex );
						break;

					case dap::ImGuiTilemapEditor::Mode::Erase:
						targetTilemap->SetTileWorld ( mouseWorldPos, dap::Tilemap::TILE_EMPTY);
						break;
				}
			}
		}
	}
}

void dap::ImGuiTilemapEditor::SetTilemap ( dap::Tilemap* map ) {
	targetTilemap = map;
}
