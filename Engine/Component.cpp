#include "Component.h"

#include "GameObject.h"
#include "FactoryMetaData.h"
#include "imgui.h"

dap::Component::~Component () {
	BaseEnd ();
	if ( owner )
		owner->RemoveComponent ( this, false );
}

void dap::Component::BaseCollision ( dap::RectCollider* other ) {
	Collision ( other );
}

void dap::Component::BaseEnd () {
	End ();
}

void dap::Component::BaseInput () {
	Input ();
}

void dap::Component::BaseStart () {
	if ( hasStarted )
		return;
	hasStarted = true;
	Start ();
}

void dap::Component::BaseTick () {
	Tick ();
}

dap::GameObject* const dap::Component::GetOwner () {
	return owner;
}

bool dap::Component::GetEnabled () {
	return isEnabled;
}

void dap::Component::ImGuiWidgetsBegin () {
	dap::FactoryMetaData* metaData = GetMetaData ( typeid( *this ).name () );
	if ( ImGui::Checkbox ( metaData->Name.c_str (), &isEnabled ) )
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
	nlohmann::json json = dap::JSONObject::Serialize ();
	json["enabled"] = isEnabled;
	return json;
}