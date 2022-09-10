#include "Component.h"

#include "GameObject.h"
#include "FactoryMetaData.h"
#include "imgui.h"

std::vector<dap::Component*> dap::Component::AllComponents = {};

dap::Component::Component () {
	AllComponents.push_back ( this );
}

dap::Component::~Component () {
	BaseEnd ();
	if ( owner )
		owner->RemoveComponent ( this, false );
	AllComponents.erase ( std::remove ( AllComponents.begin (), AllComponents.end (), this), AllComponents.end () );
}

void dap::Component::BaseCollision ( dap::CollisionData& data ) {
	Collision ( data );
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

const std::vector<dap::Component*>* dap::Component::GetAllComponents () {
	return &AllComponents;
}

dap::GameObject* const dap::Component::GetOwner () const {
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