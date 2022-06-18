#pragma once

#include <chrono>

namespace dap {
	struct InputEvent {
		InputEvent ( int key, int scancode, int action, int mods );

		int key = 0;
		int scancode = 0;
		int action = 0;
		int mods = 0;
		bool held = false;
		std::chrono::steady_clock::time_point pressTime = std::chrono::steady_clock::now ();
		std::chrono::steady_clock::time_point releaseTime = std::chrono::steady_clock::now ();
		unsigned long pressFrame = 0;
		unsigned long releaseFrame = 0;
	};
}
