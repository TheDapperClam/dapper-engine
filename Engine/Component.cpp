#include "Component.h"

#include "imgui.h"

dap::GameObject* const dap::Component::GetOwner () {
	return owner;
}

bool dap::Component::GetEnabled () {
	return isEnabled;
}

std::string dap::Component::GetName () {
	return name;
}

void dap::Component::ImGuiWidgetsBegin () {
	if ( ImGui::Checkbox ( name.c_str (), &isEnabled ) )
		SetEnabled ( isEnabled );
}

void dap::Component::Initialize () {

}

void dap::Component::SetEnabled ( bool enabled ) {
	isEnabled = enabled;
}

void dap::Component::SetOwner ( dap::GameObject* const newOwner ) {
	owner = newOwner;
}

void dap::Component::Deserialize ( const nlohmann::json* json ) {
	SetEnabled ( json->at ( "enabled" ) );
}

nlohmann::json dap::Component::Serialize () {
	nlohmann::json json {
		{ "enabled", isEnabled }
	};
	return json;
}