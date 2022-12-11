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

void dap::Component::Deserialize ( const nlohmann::json* json ) {
	SetEnabled ( json->at ( "enabled" ) );
}

bool dap::Component::GetCanBeRemoved () {
	return canBeRemoved;
}

bool dap::Component::GetCanBeToggled () {
	return canBeToggled;
}

const std::vector<dap::Component*>* dap::Component::GetComponentsToProcess () {
	return &ComponentsToProcess;
}

bool dap::Component::GetEnabled () {
	return isEnabled;
}

dap::GameObject* const dap::Component::GetOwner () const {
	return owner;
}

void dap::Component::ImGuiWidgetsBegin () {
	const std::string CLASS_NAME = typeid( *this ).name ();
	dap::FactoryMetaData* metaData = GetMetaData ( CLASS_NAME );
	if ( canBeToggled && ImGui::Checkbox ( "##enabled", &isEnabled ) )
		SetEnabled ( isEnabled );
	ImGui::SameLine ();
	ImGui::Text ( metaData ? metaData->Name.c_str () : ( "UNREGISTERED_COMP - " + CLASS_NAME ).c_str () );
}

void dap::Component::Initialize () {

}

nlohmann::json dap::Component::Serialize () {
	nlohmann::json json = dap::JSONObject::Serialize ();
	json["enabled"] = isEnabled;
	return json;
}

void dap::Component::SetEnabled ( bool enabled ) {
	isEnabled = enabled;
}

void dap::Component::SetOwner ( dap::GameObject* const newOwner ) {
	owner = newOwner;
}