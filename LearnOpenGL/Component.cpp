#include "Component.h"

void dap::Component::Initialize () {

}

void dap::Component::SetOwner ( dap::GameObject* const newOwner ) {
	owner = newOwner;
}

dap::GameObject* const dap::Component::GetOwner () {
	return owner;
}

void dap::Component::Deserialize ( const nlohmann::json* json ) {

}

const nlohmann::json dap::Component::Serialize () {
	return nlohmann::json ();
}