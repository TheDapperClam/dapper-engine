#include "ImGuiFileBrowser.h"

#include <filesystem>
#include "imgui.h"
#include "imgui_stdlib.h"
#include <vector>

#include <iostream>

dap::ImGuiFileBrowser::ImGuiFileBrowser ( std::string title ) : dap::ImGuiWindow::ImGuiWindow ( title ) {
	windowFlags = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;
	currentDirectory = std::filesystem::current_path ().string ();
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

	for ( int i = 0; i < directoryList.size (); i++ ) {
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

	if ( ImGui::BeginTable ( "##filebrowsertable", 1, ImGuiTableFlags_ScrollY, ImVec2(0.0f, WIN_SIZE.y - 96.0f) ) ) {
		ImGui::TableNextColumn ();
		for ( const auto& FILE : std::filesystem::directory_iterator ( currentDirectory ) ) {
			if ( ImGui::Button ( FILE.path ().filename ().string ().c_str () ) ) {
				if ( !std::filesystem::is_directory ( FILE.path () ) )
					filename = FILE.path ().filename ().string ();
				else
					currentDirectory = FILE.path ().string ();
			}
		}
		ImGui::EndTable ();
	}
	ImGui::Separator ();

	if ( ImGui::Button ( "Okay" ) ) {
		std::cout << GetPath () << std::endl;
		SetShow ( false );
	}

	ImGui::SameLine ();
	if ( ImGui::Button ( "Cancel" ) ) {
		SetShow ( false );
	}

	ImGui::SameLine ();
	ImGui::InputText ( "##filenameinput", &filename );
}

std::string dap::ImGuiFileBrowser::GetPath () {
	const bool FILENAME_ENTERED = filename.find_first_not_of ( ' ' ) != std::string::npos;
	return FILENAME_ENTERED ? currentDirectory + "\\" + filename : currentDirectory;
}
