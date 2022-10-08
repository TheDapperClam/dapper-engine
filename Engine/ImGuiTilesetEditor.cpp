#include "ImGuiTilesetEditor.h"

#include "imgui.h"
#include "glm/glm.hpp"
#include "Tileset.h"
#include "TextureAtlas.h"
#include "TextureAtlasSubTexture.h"
#include <vector>

const float dap::ImGuiTilesetEditor::TILE_LIST_ELEM_SIZE = 64.0f;

dap::ImGuiTilesetEditor::ImGuiTilesetEditor ( std::string title ) : dap::ImGuiWindow::ImGuiWindow ( title ) {
	windowFlags = 0;
	targetTileset = new dap::Tileset ( "MyTileset" );
}

void dap::ImGuiTilesetEditor::ContentBegin () {
	if ( !targetTileset )
		return;
	const ImTextureID TEX_ID = (void*)*dap::TextureAtlas::GetMainAtlas ()->GetTextureObject ();
	const glm::ivec2 TILE_SIZE = targetTileset->GetTileSize ();
	const dap::TextureAtlasSubTexture* PRIMARY_TEXTURE = targetTileset->GetPrimaryTexture ();
	ImDrawList* drawList = ImGui::GetWindowDrawList ();

	if ( ImGui::Button ( "Create Tile" ) )
		targetTileset->CreateTile ();
	ImGui::SameLine ();
	if ( ImGui::Button ( "Delete Tile" ) && selectedTile ) {
		targetTileset->DeleteTile ( selectedTile );
		selectedTile = NULL;
	}

	ImGui::Separator ();
	if ( ImGui::BeginTable ( "##tiletable", 1, ImGuiTableFlags_ScrollX, ImVec2 ( 0.0f, TILE_LIST_ELEM_SIZE + 26.0f ) ) ) {
		if ( ImGui::TableNextColumn () ) {
			unsigned int imageButtonId = -1;
			for ( const auto& TILE : *targetTileset->GetAllTiles () ) {
				ImGui::PushID ( ++imageButtonId );
				const auto TEX_COORDS = TILE->GetTexCoords ();
				const ImVec2 BUTTON_POS = ImGui::GetCursorScreenPos ();
				const bool IS_SELECTED = TILE == selectedTile;
				if ( ImGui::ImageButton ( TEX_ID, ImVec2 ( TILE_LIST_ELEM_SIZE, TILE_LIST_ELEM_SIZE ), ImVec2 ( TEX_COORDS[0].x, TEX_COORDS[0].y ), ImVec2 ( TEX_COORDS[2].x, TEX_COORDS[2].y ) ) )
					selectedTile = TILE;
				if ( IS_SELECTED ) {
					drawList->AddRect (
						BUTTON_POS,
						ImVec2 ( BUTTON_POS.x + TILE_LIST_ELEM_SIZE + 8.0f, BUTTON_POS.y + TILE_LIST_ELEM_SIZE + 8.0f ),
						IM_COL32 ( 255, 255, 0, 255 )
					);
				}
				ImGui::SameLine ();
				ImGui::PopID ();
			}
		}
		ImGui::EndTable ();
		ImGui::Separator ();
	}

	ImGui::Columns ( 2 );
	if ( PRIMARY_TEXTURE ) {
		const auto TEX_COORDS = PRIMARY_TEXTURE->GetTexCoords ();
		const ImVec2 IM_CURSOR_POS = ImGui::GetCursorScreenPos ();
		ImGui::Image ( TEX_ID, ImVec2 ( PRIMARY_TEXTURE->ImageWidth, PRIMARY_TEXTURE->ImageHeight ), ImVec2 ( TEX_COORDS[0].x, TEX_COORDS[0].y ), ImVec2 ( TEX_COORDS[2].x, TEX_COORDS[2].y ) );
		if ( selectedTile ) {
			drawList->AddRect (
				ImVec2 ( IM_CURSOR_POS.x + selectedTile->ImageX, IM_CURSOR_POS.y + selectedTile->ImageY ),
				ImVec2 ( IM_CURSOR_POS.x + TILE_SIZE.x + selectedTile->ImageX, IM_CURSOR_POS.y + TILE_SIZE.y + selectedTile->ImageY ),
				IM_COL32 ( 255, 255, 0, 255 )
			);
		}
	}

	ImGui::NextColumn ();
	if ( ImGui::BeginCombo ( "Texture", PRIMARY_TEXTURE ? PRIMARY_TEXTURE->Name.c_str () : "" ) ) {
		for ( const auto& TEX_PAIR : *dap::TextureAtlas::GetMainAtlas ()->GetAllSubTextures () ) {
			bool isSelected = TEX_PAIR.second == PRIMARY_TEXTURE;
			if ( ImGui::Selectable ( TEX_PAIR.first.c_str (), &isSelected ) )
				targetTileset->SetPrimaryTexture ( TEX_PAIR.second );
		}
		ImGui::EndCombo ();
	}

	glm::ivec2 tileSize = targetTileset->GetTileSize ();
	if ( ImGui::DragInt2 ( "Tile Size", &tileSize.x, 1.0f, 0, INT_MAX ) )
		targetTileset->SetTileSize ( tileSize );

	if ( PRIMARY_TEXTURE && selectedTile && ImGui::DragInt2 ( "Tile Position", &selectedTile->ImageX ) )
		selectedTile->MapToSubTexture ( PRIMARY_TEXTURE );
	ImGui::Columns ();
}

void dap::ImGuiTilesetEditor::SetTargetTileset ( dap::Tileset* target ) {
	targetTileset = target;
}
