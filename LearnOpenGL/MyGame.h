#pragma once

#include "Game.h"

namespace dap {
	class GameObject;

	class MyGame : public dap::Game {
	public:
		MyGame ( std::string projectDir );
	public:
		virtual void End () override;
		virtual void Input () override;
		virtual void Start () override;
		virtual void Tick ( const float time, const float delta ) override;
	private:
		dap::GameObject* myObject;
	};
}
