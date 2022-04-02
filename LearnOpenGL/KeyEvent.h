#pragma once

#include <chrono>

namespace dap {
	struct KeyEvent {
		KeyEvent ( int key, int scancode, int action, int mods );
		int key = 0;
		int scancode = 0;
		int action = 0;
		int mods = 0;
		bool held = false;
		std::chrono::steady_clock::time_point pressTime;
		std::chrono::steady_clock::time_point releaseTime;
		unsigned long pressFrame;
		unsigned long releaseFrame;
	};
}
