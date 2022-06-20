#include "Game.h"
#include "Input.h"
#include "JSONobject.h"
#include "Scene.h"

dap::Game::~Game () {
	dap::Input::Clear ();
	dap::JSONobject::ClearFactories ();
	dap::Scene::ClearAll ();
}

void dap::Game::Close () {
	shouldClose = true;
}

bool dap::Game::ShouldClose () {
	return shouldClose;
}
