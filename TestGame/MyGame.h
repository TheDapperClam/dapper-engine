#pragma once

#include "Game.h"
#include "glm/gtc/matrix_transform.hpp"
#include <string>

namespace dap {
	class GameObject;
	class Scene;

	class MyGame : public dap::Game {
	private:
		static const float TIME_MAX;
		static const std::string APPLE_PREFAB;

	private:
		dap::Scene* scene = NULL;
		dap::GameObject* appleObject = NULL;
		dap::GameObject* cameraObject = NULL;
		dap::GameObject* labelObject = NULL;

		bool isClicking = false;
		bool isGameStarted = false;
		glm::vec3 mousePoint;
		unsigned int applesCollected = 0;
		float timeStart = 0.0f;

	public:
		virtual void End () override;
		virtual void Input () override;
		virtual void Start () override;
		virtual void Tick () override;
	};
}
