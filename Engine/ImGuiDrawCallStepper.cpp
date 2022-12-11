#include "ImGuiDrawCallStepper.h"

#include <imgui.h>

bool dap::ImGuiDrawCallStepper::IsStepping = false;
unsigned int dap::ImGuiDrawCallStepper::CurrentStep = 0;

dap::ImGuiDrawCallStepper::ImGuiDrawCallStepper ( std::string title ) : dap::ImGuiWindow::ImGuiWindow ( title ) {

}

unsigned int dap::ImGuiDrawCallStepper::GetCurrentStep () {
	return CurrentStep;
}

bool dap::ImGuiDrawCallStepper::GetIsStepping () {
	return IsStepping;
}

void dap::ImGuiDrawCallStepper::ContentBegin () {
	ImGui::Checkbox ( "Enabled", &IsStepping );

	if ( ImGui::Button ( "< Previous" ) && CurrentStep > 0 )
		CurrentStep--;
	ImGui::SameLine ();
	if ( ImGui::Button ( "Next >" ) && CurrentStep < UINT_MAX )
		CurrentStep++;

	ImGui::Text ( ( "Step: " + std::to_string ( CurrentStep ) ).c_str () );
}
