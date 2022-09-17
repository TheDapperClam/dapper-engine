#include "Input.h"

#include "glm/glm.hpp"
#include "InputEvent.h"
#include "Rendering.h"

std::unordered_map<int, dap::InputEvent*> dap::Input::pressedKeys = {};
glm::vec2 dap::Input::mousePos = glm::vec2( INT_MIN, INT_MIN );
glm::vec2 dap::Input::mousePosDelta = mousePos;
glm::vec2 dap::Input::mouseScroll = glm::vec2 ( 0.0f, 0.0f );

void dap::Input::AddKey ( const int key, dap::InputEvent* inputEvent ) {
	if ( pressedKeys.count ( key ) )
		return;
	pressedKeys.insert ( std::pair<int, dap::InputEvent*> ( key, inputEvent ) );
}

void dap::Input::ClearMouse () {
	mousePosDelta = mousePos;
	mouseScroll = glm::vec2 ( 0.0f, 0.0f );
}

void dap::Input::DeleteInputs () {
	for ( auto const& pair : pressedKeys ) {
		const int key = (int)pair.first;
		if ( pressedKeys[key] )
			continue;
		delete pressedKeys[key];
		pressedKeys[key] = NULL;
	}
	pressedKeys.clear ();
}

glm::vec2 dap::Input::GetMousePosition () {
	return mousePos;
}

glm::vec2 dap::Input::GetMouseScroll () {
	return mouseScroll;
}

glm::vec2 dap::Input::GetMouseVelocity () {
	const glm::vec2 DIFF = mousePos - mousePosDelta;
	return DIFF;
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

void dap::Input::SetMousePos ( float x, float y ) {
	mousePosDelta = mousePos;
	mousePos = glm::vec2 ( x, y );
}

void dap::Input::SetMouseScroll ( float x, float y ) {
	mouseScroll = glm::vec2 ( x, y );
}
