#include "Input.h"
#include "Rendering.h"

std::map<int, dap::KeyEvent*> dap::Input::pressedKeys = {};

void dap::Input::AddKey ( const int key, dap::KeyEvent* inputEvent ) {
	if ( pressedKeys.count ( key ) )
		return;
	pressedKeys.insert ( std::pair<int, dap::KeyEvent*> ( key, inputEvent ) );
}

void dap::Input::SetKeyHeld ( const unsigned int frame, const int key, const bool held ) {
	if ( !pressedKeys.count ( key ) || pressedKeys[key]->held == held )
		return;
	pressedKeys[key]->held = held;
	if ( held )
		pressedKeys[key]->pressFrame = frame;
	else
		pressedKeys[key]->releaseFrame = frame;
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

const bool dap::Input::KeyPressed ( const int key ) {
	if ( !pressedKeys.count ( key ) )
		return false;
	return pressedKeys[key]->held && pressedKeys[key]->pressFrame == dap::Rendering::GetFrame ();
}

const bool dap::Input::KeyPressedRecently ( const int key, const float threshold ) {
	if ( !pressedKeys.count ( key ) )
		return false;
	return std::chrono::duration_cast<std::chrono::duration<float>>( std::chrono::steady_clock::now () - pressedKeys[key]->pressTime ).count () <= threshold;
}

const bool dap::Input::KeyReleased ( const int key ) {
	if ( !pressedKeys.count ( key ) )
		return false;
	return !pressedKeys[key]->held && pressedKeys[key]->releaseFrame == dap::Rendering::GetFrame ();
}

const bool dap::Input::KeyReleasedRecently ( const int key, const float threshold ) {
	if ( !pressedKeys.count ( key ) )
		return false;
	return std::chrono::duration_cast<std::chrono::duration<float>>( std::chrono::steady_clock::now () - pressedKeys[key]->releaseTime ).count () <= threshold;
}

const bool dap::Input::KeyHeld ( const int key ) {
	if ( !pressedKeys.count ( key ) )
		return false;
	return pressedKeys[key]->held;
}
