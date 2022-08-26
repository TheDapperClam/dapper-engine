#include "Input.h"

#include "InputEvent.h"
#include "Rendering.h"

std::map<int, dap::InputEvent*> dap::Input::pressedKeys = {};
glm::vec3 dap::Input::mousePos = glm::vec3 ( INT_MIN, INT_MIN, INT_MIN );

void dap::Input::AddKey ( const int key, dap::InputEvent* inputEvent ) {
	if ( pressedKeys.count ( key ) )
		return;
	pressedKeys.insert ( std::pair<int, dap::InputEvent*> ( key, inputEvent ) );
}

void dap::Input::SetHeld ( const unsigned int frame, const int key, const bool held ) {
	if ( !pressedKeys.count ( key ) || pressedKeys[key]->held == held )
		return;
	pressedKeys[key]->held = held;
	if ( held ) {
		pressedKeys[key]->pressFrame = frame;
		pressedKeys[key]->pressTime = std::chrono::steady_clock::now ();
	} else {
		pressedKeys[key]->releaseFrame = frame;
		pressedKeys[key]->releaseTime = std::chrono::steady_clock::now ();
	}
}

void dap::Input::Clear () {
	for ( auto const &pair : pressedKeys ) {
		const int key = (int)pair.first;
		if ( pressedKeys[key] )
			continue;
		delete pressedKeys[key];
		pressedKeys[key] = NULL;
	}
	pressedKeys.clear ();
}

glm::vec3 dap::Input::GetMousePos () {
	return mousePos;
}

bool dap::Input::Held ( const int key ) {
	if ( !pressedKeys.count ( key ) )
		return false;
	return pressedKeys[key]->held;
}

bool dap::Input::Pressed ( const int key ) {
	if ( !pressedKeys.count ( key ) )
		return false;
	return pressedKeys[key]->held && pressedKeys[key]->pressFrame == dap::Rendering::GetFrame ();
}

bool dap::Input::PressedRecently ( const int key, const float threshold ) {
	if ( !pressedKeys.count ( key ) )
		return false;
	return std::chrono::duration_cast<std::chrono::duration<float>>( std::chrono::steady_clock::now () - pressedKeys[key]->pressTime ).count () <= threshold;
}

bool dap::Input::Released ( const int key ) {
	if ( !pressedKeys.count ( key ) )
		return false;
	return !pressedKeys[key]->held && pressedKeys[key]->releaseFrame == dap::Rendering::GetFrame ();
}

bool dap::Input::ReleasedRecently ( const int key, const float threshold ) {
	if ( !pressedKeys.count ( key ) )
		return false;
	return std::chrono::duration_cast<std::chrono::duration<float>>( std::chrono::steady_clock::now () - pressedKeys[key]->releaseTime ).count () <= threshold;
}

void dap::Input::SetMousePos ( float x, float y ) {
	mousePos = glm::vec3 ( x, y, 0.0f );
}
