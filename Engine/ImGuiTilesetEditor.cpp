#include "ImGuiTilesetEditor.h"

#include "Event.h"
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
	windowFlags = ImGuiWindowFlags_MenuBar;
	fileBrowser = new dap::ImGuiFileBrowser ( dap::ImGuiFileBrowser::Mode::Open );
	fileBrowser->SetFilters ( { ".dts" } );
	fileBrowser->GetEventOnRespond ()->Subscribe<dap::ImGuiTilesetEditor> ( &dap::ImGuiTilesetEditor::OnFileBrowserRespond, this );
}

dap::ImGuiTilesetEditor::~ImGuiTilesetEditor () {
	delete fileBrowser;
}

void dap::ImGuiTilesetEditor::ContentBegin () {
	ImDrawList* drawList = ImGui::GetWindowDrawList ();

	if ( ImGui::BeginMenuBar () ) {
		if ( ImGui::BeginMenu ( "File" ) ) {
			if ( ImGui::MenuItem ( "New" ) )
				targetTileset = dap::Tileset ( "", false );
			ImGui::Separator ();
			if ( ImGui::MenuItem ( "Save" ) ) {
				const std::string FILE_PATH = targetTileset.GetFilePath ();
				if ( FILE_PATH.empty () ) {
					fileBrowser->SetMode ( dap::ImGuiFileBrowser::Mode::Save );
					fileBrowser->SetShow ( true );
				} else
					targetTileset.SaveFile ( FILE_PATH );
			}
			if ( ImGui::MenuItem ( "Save As" ) ) {
				fileBrowser->SetMode ( dap::ImGuiFileBrowser::Mode::Save );
				fileBrowser->SetShow ( true );
			}
			ImGui::Separator ();
			if ( ImGui::MenuItem ( "Open" ) ) {
				fileBrowser->SetMode ( dap::ImGuiFileBrowser::Mode::Open );
				fileBrowser->SetShow ( true );
			}
			ImGui::EndMenu ();
		}
		ImGui::EndMenuBar ();
	}

	const ImTextureID TEX_ID = (void*)*dap::TextureAtlas::GetMainAtlas ()->GetTextureObject ();
	const glm::ivec2 TILE_SIZE = targetTileset.GetTileSize ();
	const dap::TextureAtlasSubTexture* PRIMARY_TEXTURE = targetTileset.GetPrimaryTexture ();

	if ( ImGui::Button ( "Create Tile" ) )
		targetTileset.CreateTile ();

	ImGui::SameLine ();
	if ( ImGui::Button ( "Delete Tile" ) && selectedTile ) {
		targetTileset.DeleteTile ( selectedTile );
		selectedTile = NULL;
	}

	if ( PRIMARY_TEXTURE ) {
		ImGui::SameLine ();
		if ( ImGui::Button ( "Auto Tile" ) ) {
			for ( int y = 0; y + TILE_SIZE.y < PRIMARY_TEXTURE->ImageHeight; y += TILE_SIZE.y ) {
				for ( int x = 0; x + TILE_SIZE.x < PRIMARY_TEXTURE->ImageWidth; x += TILE_SIZE.x ) {
					targetTileset.CreateTile ( x, y );
				}
			}
		}
	}

	if ( ImGui::BeginTable ( "##tiletable", 1, ImGuiTableFlags_ScrollX, ImVec2 ( 0.0f, TILE_LIST_ELEM_SIZE + 26.0f ) ) ) {
		if ( ImGui::TableNextColumn () ) {
			unsigned int imageButtonId = -1;
			for ( const auto& TILE : *targetTileset.GetAllTiles () ) {
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
		for ( const auto& TILE : *targetTileset.GetAllTiles () ) {
			const bool IS_SELECTED = TILE == selectedTile;
			const ImVec2 RECT_MIN = ImVec2 ( IM_CURSOR_POS.x + TILE->ImageX, IM_CURSOR_POS.y + TILE->ImageY );
			const ImVec2 RECT_MAX = ImVec2 ( IM_CURSOR_POS.x + TILE_SIZE.x + TILE->ImageX, IM_CURSOR_POS.y + TILE_SIZE.y + TILE->ImageY );
			const ImVec2 MOUSE_POS = ImGui::GetMousePos ();
			const bool MOUSE_OVER = MOUSE_POS.x > RECT_MIN.x && MOUSE_POS.y > RECT_MIN.y && MOUSE_POS.x < RECT_MAX.x&& MOUSE_POS.y < RECT_MAX.y;
			const ImU32 RECT_COLOR = IS_SELECTED || MOUSE_OVER ? IM_COL32 ( 255, 255, 0, 255 ) : IM_COL32 ( 0, 0, 0, 100 );
			drawList->AddRect ( RECT_MIN, RECT_MAX, RECT_COLOR );

			if ( !IS_SELECTED && MOUSE_OVER && ImGui::IsMouseClicked ( ImGuiMouseButton_Left ) )
				selectedTile = TILE;
		}
	}

	ImGui::NextColumn ();
	if ( ImGui::BeginCombo ( "Texture", PRIMARY_TEXTURE ? PRIMARY_TEXTURE->Name.c_str () : "" ) ) {
		for ( const auto& TEX_PAIR : *dap::TextureAtlas::GetMainAtlas ()->GetAllSubTextures () ) {
			bool isSelected = TEX_PAIR.second == PRIMARY_TEXTURE;
			if ( ImGui::Selectable ( TEX_PAIR.first.c_str (), &isSelected ) )
				targetTileset.SetPrimaryTexture ( TEX_PAIR.second );
		}
		ImGui::EndCombo ();
	}

	glm::ivec2 tileSize = targetTileset.GetTileSize ();
	if ( ImGui::DragInt2 ( "Tile Size", &tileSize.x, 1.0f, 0, INT_MAX ) )
		targetTileset.SetTileSize ( tileSize );

	if ( PRIMARY_TEXTURE && selectedTile && ImGui::DragInt2 ( "Tile Position", &selectedTile->ImageX ) )
		selectedTile->MapToSubTexture ( PRIMARY_TEXTURE );
	ImGui::Columns ();
}

void dap::ImGuiTilesetEditor::OnFileBrowserRespond ( dap::ImGuiFileBrowser::Response response, std::string& path ) {
	if ( response != dap::ImGuiFileBrowser::Response::OK )
		return;
	switch ( fileBrowser->GetMode () ) {
		case dap::ImGuiFileBrowser::Mode::Save:
			targetTileset.SaveFile ( path );
			break;
		case dap::ImGuiFileBrowser::Mode::Open:
			targetTileset.LoadFile ( path );
			break;
	}
}
