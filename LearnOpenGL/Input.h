#pragma once

#include "KeyEvent.h"
#include "glm/gtc/matrix_transform.hpp"
#include <map>

namespace dap {
	class Input final {
	public:
		const static bool KeyPressed ( const int key );
		const static bool KeyPressedRecently ( const int key, const float threshold );
		const static bool KeyReleased ( const int key );
		const static bool KeyReleasedRecently ( const int key, const float threshold );
		const static bool KeyHeld ( const int key );
		static glm::vec3 GetMousePos ();
		static void Clear ();
	private:
		friend class Rendering;
		static void AddKey ( const int key, dap::KeyEvent* inputEvent );
		static void SetKeyHeld ( const unsigned int frame, const int key, const bool held );
		static void SetMousePos ( float x, float y );
		static std::map<int, dap::KeyEvent*> pressedKeys;
		static glm::vec3 mousePos;
	};
}
