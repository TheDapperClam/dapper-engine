#include "Controller.h"

void dap::Controller::Deserialize ( const nlohmann::json* json ) {
	dap::Component::Deserialize ( json );
}

void dap::Controller::BaseCollision ( dap::RectCollider* other ) {
	Collision ( other );
}

void dap::Controller::BaseEnd () {
	End ();
}

void dap::Controller::BaseInput () {
	Input ();
}

void dap::Controller::BaseStart () {
	if ( hasStarted )
		return;
	hasStarted = true;
	Start ();
}

void dap::Controller::BaseTick () {
	Tick ();
}

nlohmann::json dap::Controller::Serialize () {
	nlohmann::json json = dap::Component::Serialize ();
	JSON_APPEND_TYPE ( json );
	return json;
}
