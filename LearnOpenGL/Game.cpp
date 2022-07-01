#include "Game.h"
#include "Input.h"
#include "JSONobject.h"
#include "Rendering.h"
#include "Resources.h"
#include "Scene.h"

dap::Game::~Game () {
	dap::Input::Clear ();
	dap::JSONobject::ClearFactories ();
	dap::Scene::ClearAll ();
}

void dap::Game::Close () {
	shouldClose = true;
}

void dap::Game::Run ( unsigned int windowWidth, unsigned int windowHeight, unsigned int tickRate ) {
	dap::JSONobject::RegisterEngineObjects ();
	dap::Rendering::Start ( windowWidth, windowHeight, tickRate, this );
}

bool dap::Game::ShouldClose () {
	return shouldClose;
}
