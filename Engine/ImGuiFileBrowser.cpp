#include "ImGuiFileBrowser.h"

#include <filesystem>
#include "imgui.h"
#include "imgui_stdlib.h"
#include <vector>

const int dap::ImGuiFileBrowser::DIRECTORY_DISPLAY_SIZE = 5;

dap::ImGuiFileBrowser::ImGuiFileBrowser ( dap::ImGuiFileBrowser::Mode mode ) : dap::ImGuiWindow::ImGuiWindow ( "##window" ) {
	advertise = false;
	windowFlags = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;
	currentDirectory = std::filesystem::current_path ().string ();
	SetMode ( mode );
}

void dap::ImGuiFileBrowser::ContentBegin () {
	std::vector<std::string> directoryList;
	unsigned int lastSlashPos = 0;
	bool runDirectorySplit = true;
	const ImVec2 WIN_SIZE = ImGui::GetWindowSize ();

	if ( ImGui::Button ( "^##updirectorybutton" ) ) {
		const unsigned int FINAL_DIR_NODE_START = currentDirectory.find_last_of ( '\\' );
		currentDirectory = currentDirectory.substr ( 0, FINAL_DIR_NODE_START );
	}

	do {
		const unsigned int NEXT_SLASH_POS = currentDirectory.find ( '\\', lastSlashPos );
		const std::string DIR_NODE = currentDirectory.substr ( lastSlashPos, NEXT_SLASH_POS - lastSlashPos );
		directoryList.push_back ( DIR_NODE );
		lastSlashPos = NEXT_SLASH_POS + 1;
		if ( NEXT_SLASH_POS >= currentDirectory.length () )
			break;
	} while ( runDirectorySplit );

	for ( int i = std::max ( 0, (int) directoryList.size () - DIRECTORY_DISPLAY_SIZE - 1 ); i < directoryList.size (); i++ ) {
		const std::string& DIR_NODE = directoryList[i];
		ImGui::SameLine ();
		if ( ImGui::Button ( DIR_NODE.c_str () ) ) {
			currentDirectory = "";
			for ( int j = 0; j <= i; j++ )
				currentDirectory += directoryList[j] + ( j < i ? "\\" : "" );
			break;
		}
	}
	ImGui::Separator ();

	if ( ImGui::BeginTable ( "##filebrowsertable", 1, ImGuiTableFlags_ScrollY, ImVec2(0.0f, std::max(1.0f, WIN_SIZE.y - 96.0f)) ) ) {
		ImGui::TableNextColumn ();
		for ( const auto& DIR_ENTRY : std::filesystem::directory_iterator ( currentDirectory ) ) {
			const bool IS_FILE = !std::filesystem::is_directory ( DIR_ENTRY.path () );
			if ( IS_FILE && !filters.empty () && std::find ( filters.begin (), filters.end (), DIR_ENTRY.path ().extension () ) == filters.end () )
				continue;
			if ( ImGui::Button ( DIR_ENTRY.path ().filename ().string ().c_str () ) ) {
				if ( IS_FILE )
					filename = DIR_ENTRY.path ().filename ().string ();
				else
					currentDirectory = DIR_ENTRY.path ().string ();
			}
		}
		ImGui::EndTable ();
	}
	ImGui::Separator ();

	if ( !GetShow () )
		Respond ( dap::ImGuiFileBrowser::Response::None );

	if ( filename.find_first_not_of ( ' ' ) != std::string::npos ) {
		if ( ImGui::Button ( title.c_str () ) )
			Respond ( dap::ImGuiFileBrowser::Response::OK );
		ImGui::SameLine ();
	}

	if ( ImGui::Button ( "Cancel" ) )
		Respond ( dap::ImGuiFileBrowser::Response::Cancel );

	ImGui::SameLine ();
	ImGui::InputText ( ( filtersDisplayString + "##filenameinput" ).c_str (), &filename, mode == dap::ImGuiFileBrowser::Mode::Open ? ImGuiInputTextFlags_ReadOnly : 0 );
}

void dap::ImGuiFileBrowser::Respond ( dap::ImGuiFileBrowser::Response response ) {
	const bool FILENAME_ENTERED = filename.find_first_not_of ( ' ' ) != std::string::npos;
	if ( !FILENAME_ENTERED )
		return;

	std::string finalPath = FILENAME_ENTERED ? currentDirectory + "\\" + filename : currentDirectory;
	if ( filters.size () > 0 && !std::filesystem::path ( filename ).has_extension () )
		finalPath += filters[0];
	eventOnRespond.Invoke ( response, finalPath );
	filename = "";
	SetShow ( false );
}

dap::Event<dap::ImGuiFileBrowser::Response, std::string&>* const dap::ImGuiFileBrowser::GetEventOnRespond () {
	return &eventOnRespond;
}

dap::ImGuiFileBrowser::Mode dap::ImGuiFileBrowser::GetMode () {
	return mode;
}

void dap::ImGuiFileBrowser::SetFilters ( std::vector<std::string> newFilters ) {
	filters = newFilters;
	filtersDisplayString = "";
	for ( int i = 0; i < newFilters.size (); i++ ) {
		const std::string FILTER = newFilters[i];
		filtersDisplayString += FILTER + (i + 1 < newFilters.size () ? "|" : "" );
	}
}

void dap::ImGuiFileBrowser::SetMode ( dap::ImGuiFileBrowser::Mode mode ) {
	this->mode = mode;
	title = mode == dap::ImGuiFileBrowser::Mode::Save ? "Save" : "Open";
}
