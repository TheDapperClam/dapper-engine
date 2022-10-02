#include "ImGuiWindow.h"

std::vector<dap::ImGuiWindow*> dap::ImGuiWindow::AllWindows = {};

dap::ImGuiWindow::ImGuiWindow ( std::string title ) {
	this->title = title;
	AllWindows.push_back ( this );
}

void dap::ImGuiWindow::Begin () {
	if ( !shouldShow )
		return;
	if ( !ImGui::Begin ( title.c_str (), &shouldShow, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_AlwaysVerticalScrollbar ) ) {
		ImGui::End ();
		return;
	}
	ContentBegin ();
	ImGui::End ();
}

void dap::ImGuiWindow::ClearAllWindows () {
	for ( const auto& WINDOW : AllWindows ) {
		delete WINDOW;
	}
	AllWindows.clear ();
}

const std::vector<dap::ImGuiWindow*>* dap::ImGuiWindow::GetAllWindows () {
	return &AllWindows;
}

bool* const dap::ImGuiWindow::GetShow () {
	return &shouldShow;
}

std::string dap::ImGuiWindow::GetTitle () {
	return title;
}

void dap::ImGuiWindow::SetShow ( bool show ) {
	shouldShow = show;
}
