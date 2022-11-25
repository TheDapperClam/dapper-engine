#include "ImGuiWindow.h"

#include "glm/glm.hpp"
#include "Rendering.h"

const int dap::ImGuiWindow::MARGIN_X = 50;
const int dap::ImGuiWindow::MARGIN_Y = 50;

std::vector<dap::ImGuiWindow*> dap::ImGuiWindow::AllWindows = {};

dap::ImGuiWindow::ImGuiWindow ( std::string title ) {
	this->title = title;
	AllWindows.push_back ( this );
}

void dap::ImGuiWindow::Begin () {
	if ( !shouldShow )
		return;
	const glm::vec2 MAX_WINDOW_SIZE = dap::Rendering::GetWindowSize ();
	ImGui::SetNextWindowSizeConstraints ( ImVec2 ( 0.0f, 0.0f ), ImVec2 ( MAX_WINDOW_SIZE.x - MARGIN_X * 2, MAX_WINDOW_SIZE.y - MARGIN_Y * 2 ) );
	if ( !ImGui::Begin ( title.c_str (), &shouldShow, windowFlags ) ) {
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

bool dap::ImGuiWindow::GetAdvertise () {
	return advertise;
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

void dap::ImGuiWindow::SetAdvertise ( bool shouldAdvertise ) {
	advertise = shouldAdvertise;
}

void dap::ImGuiWindow::SetShow ( bool show ) {
	shouldShow = show;
}
