#pragma once

#include <GLFW/glfw3.h>
#include <iostream>
#include "Camera.h"
#include "GameObject.h"
#include "Input.h"
#include <map>
#include "Sprite.h"
#include "Texture.h"

namespace dap {
	class Game {
	public:
		~Game ();
		void Close ();
		const bool ShouldClose ();
		virtual void End () = 0;
		virtual void Input () = 0;
		virtual void Start () = 0;
		virtual void Tick ( const float time, const float delta ) = 0;
	private:
		bool shouldClose = false;
	};
}
