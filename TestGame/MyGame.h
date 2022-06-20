#pragma once

#include "Game.h"
#include "glm/gtc/matrix_transform.hpp"

namespace dap {
	class GameObject;
	class Scene;

	class MyGame : public dap::Game {
	private:
		dap::Scene* scene = NULL;
		dap::GameObject* appleObject = NULL;
		dap::GameObject* cameraObject = NULL;
		dap::GameObject* playerObject = NULL;

		unsigned int applesCollected = 0;
		float playerMoveSpeed = 1.0f;
		glm::vec2 playerMoveInput = glm::vec2 ( 0, 0 );
		glm::vec2 playerVelocity = glm::vec2 ( 0, 0 );

	public:
		virtual void End () override;
		virtual void Input () override;
		virtual void Start () override;
		virtual void Tick () override;
	};
}
