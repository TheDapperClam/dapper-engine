#pragma once

#include "glm/gtc/matrix_transform.hpp"
#include <map>

namespace dap {
	struct InputEvent;

	class Input final {
		friend class Rendering;

	private:
		static std::map<int, dap::InputEvent*> pressedKeys;
		static glm::vec3 mousePos;

	public:
		static void Clear ();
		static glm::vec3 GetMousePos ();
		static bool Held ( const int key );
		static bool Pressed ( const int key );
		static bool PressedRecently ( const int key, const float threshold );
		static bool Released ( const int key );
		static bool ReleasedRecently ( const int key, const float threshold );

	private:
		static void AddKey ( const int key, dap::InputEvent* inputEvent );
		static void SetHeld ( const unsigned int frame, const int key, const bool held );
		static void SetMousePos ( float x, float y );
	};
}
