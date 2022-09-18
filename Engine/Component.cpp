#include "Component.h"

#include "GameObject.h"
#include "FactoryMetaData.h"
#include "imgui.h"

std::vector<dap::Component*> dap::Component::ComponentsToProcess = {};

dap::Component::Component ( bool processGameFunctions ) : PROCESS_GAME_FUNCTIONS ( processGameFunctions ) {
	if ( processGameFunctions )
		ComponentsToProcess.push_back ( this );
}

dap::Component::~Component () {
	//this->BaseEnd ();
	if ( owner )
		owner->RemoveComponent ( this, false );
	if ( PROCESS_GAME_FUNCTIONS )
		ComponentsToProcess.erase ( std::remove ( ComponentsToProcess.begin (), ComponentsToProcess.end (), this ), ComponentsToProcess.end () );
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

const std::vector<dap::Component*>* dap::Component::GetComponentsToProcess () {
	return &ComponentsToProcess;
}

dap::GameObject* const dap::Component::GetOwner () const {
	return owner;
}

bool dap::Component::GetEnabled () {
	return isEnabled;
}

void dap::Component::ImGuiWidgetsBegin () {
	dap::FactoryMetaData* metaData = GetMetaData ( typeid( *this ).name () );
	if ( ImGui::Checkbox ( metaData ? metaData->Name.c_str () : "UNREGISTERED_COMP", &isEnabled ) )
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