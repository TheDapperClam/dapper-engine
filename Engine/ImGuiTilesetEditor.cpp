#include "ImGuiTilesetEditor.h"

#include "glm/glm.hpp"
#include "imgui.h"
#include "imgui_stdlib.h"
#include "Tileset.h"
#include "TextureAtlas.h"
#include "TextureAtlasSubTexture.h"
#include <vector>

const float dap::ImGuiTilesetEditor::TILE_LIST_ELEM_SIZE = 64.0f;
const std::map<std::string, dap::Tileset*>* dap::ImGuiTilesetEditor::ALL_TILESETS = dap::Tileset::GetAllTilesets ();

dap::ImGuiTilesetEditor::ImGuiTilesetEditor ( std::string title ) : dap::ImGuiWindow::ImGuiWindow ( title ) {
	windowFlags = 0;
}

void dap::ImGuiTilesetEditor::ContentBegin () {
	ImDrawList* drawList = ImGui::GetWindowDrawList ();

	if ( ImGui::Button ( "New Tileset" ) )
		isNewTilesetPopupOpen = true;
	ShowNewTilesetPopup ();

	if ( targetTileset ) {
		ImGui::SameLine ();
		if ( ImGui::Button ( "Delete Tileset" ) ) {
			delete targetTileset;
			SetTargetTileset ( NULL );
		}
	}

	if ( ImGui::BeginCombo ( "Tileset", targetTileset ? targetTileset->GetName ().c_str () : "" ) ) {
		for ( const auto& TILESET_PAIR : *ALL_TILESETS ) {
			bool isSelected = TILESET_PAIR.second == targetTileset;
			if ( ImGui::Selectable ( TILESET_PAIR.first.c_str (), &isSelected ) )
				SetTargetTileset ( TILESET_PAIR.second );
		}
		ImGui::EndCombo();
	}
	ImGui::Separator ();

	if ( targetTileset ) {
		const ImTextureID TEX_ID = (void*)*dap::TextureAtlas::GetMainAtlas ()->GetTextureObject ();
		const glm::ivec2 TILE_SIZE = targetTileset->GetTileSize ();
		const dap::TextureAtlasSubTexture* PRIMARY_TEXTURE = targetTileset->GetPrimaryTexture ();

		if ( ImGui::Button ( "Create Tile" ) )
			targetTileset->CreateTile ();

		ImGui::SameLine ();
		if ( ImGui::Button ( "Delete Tile" ) && selectedTile ) {
			targetTileset->DeleteTile ( selectedTile );
			selectedTile = NULL;
		}

		if ( PRIMARY_TEXTURE ) {
			ImGui::SameLine ();
			if ( ImGui::Button ( "Auto Tile" ) ) {
				for ( int y = 0; y + TILE_SIZE.y < PRIMARY_TEXTURE->ImageHeight; y += TILE_SIZE.y ) {
					for ( int x = 0; x + TILE_SIZE.x < PRIMARY_TEXTURE->ImageWidth; x += TILE_SIZE.x ) {
						targetTileset->CreateTile ( x, y );
					}
				}
			}
		}

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
}

void dap::ImGuiTilesetEditor::SetTargetTileset ( dap::Tileset* target ) {
	targetTileset = target;
	selectedTile = NULL;
}

void dap::ImGuiTilesetEditor::ShowNewTilesetPopup () {
	if ( !isNewTilesetPopupOpen )
		return;
	
	ImGui::OpenPopup ( "Create New Tileset" );
	if ( ImGui::BeginPopupModal ( "Create New Tileset", &isNewTilesetPopupOpen, ImGuiWindowFlags_AlwaysAutoResize ) ) {
		ImGui::InputText ( "Name", &newTilesetName );
		const bool NAME_ENTERED = newTilesetName.find_first_not_of ( ' ' ) != std::string::npos;
		const bool NAME_TAKEN = ALL_TILESETS->contains ( newTilesetName );

		if ( !NAME_ENTERED )
			ImGui::Text ( "Please enter a valid name" );
		else if ( NAME_TAKEN )
			ImGui::Text ( "Name already taken" );
		else {
			if ( ImGui::Button ( "Create" ) ) {
				targetTileset = new dap::Tileset ( newTilesetName );
				newTilesetName = "";
				isNewTilesetPopupOpen = false;
				ImGui::CloseCurrentPopup ();
			}
			ImGui::SameLine ();
		}

		if ( ImGui::Button ( "Cancel" ) ) {
			newTilesetName = "";
			isNewTilesetPopupOpen = false;
			ImGui::CloseCurrentPopup ();
		}
		ImGui::EndPopup ();
	}
}
