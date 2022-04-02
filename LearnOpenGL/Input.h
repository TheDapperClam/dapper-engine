#pragma once

#include <map>
#include "KeyEvent.h"

namespace dap {
	class Input final {
	public:
		const static bool KeyPressed ( const int key );
		const static bool KeyPressedRecently ( const int key, const float threshold );
		const static bool KeyReleased ( const int key );
		const static bool KeyReleasedRecently ( const int key, const float threshold );
		const static bool KeyHeld ( const int key );
		static void Clear ();
	private:
		friend class Rendering;
		static void AddKey ( const int key, dap::KeyEvent* inputEvent );
		static void SetKeyHeld ( const unsigned int frame, const int key, const bool held );
		static std::map<int, dap::KeyEvent*> pressedKeys;
	};
}
