#pragma once

#include "Game.h"
#include "glm/gtc/matrix_transform.hpp"

namespace dap {
	class GameObject;
	class Scene;

	class MyGame : public dap::Game {
	public:
		MyGame ( std::string projectDir );
	public:
		virtual void End () override;
		virtual void Input () override;
		virtual void Start () override;
		virtual void Tick ( const float time, const float delta ) override;
	private:
		bool wantsToClick;
		dap::Scene* scene;
		dap::GameObject* appleObject;
		dap::GameObject* cameraObject;
		unsigned int appleClicks = 0;
	};
}
