#pragma once

#include "Game.h"

namespace dap {
	class MyGame : public Game {
	public:
		virtual void End ();
		virtual void Input ();
		virtual void Start ();
		virtual void Tick ( const float time, const float delta );
	};
}
