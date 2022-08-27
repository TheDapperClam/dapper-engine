#include "Component.h"

#include "FactoryMetaData.h"
#include "imgui.h"

dap::GameObject* const dap::Component::GetOwner () {
	return owner;
}

bool dap::Component::GetEnabled () {
	return isEnabled;
}

void dap::Component::ImGuiWidgetsBegin () {
	dap::FactoryMetaData* metaData = GetMetaData ( typeid( *this ).name () );
	if ( ImGui::Checkbox ( metaData ? metaData->Name.c_str () : "NAME_NOT_FOUND", &isEnabled ) )
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