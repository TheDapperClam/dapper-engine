#include "Game.h"
#include "Input.h"
#include "JSONobject.h"
#include "Scene.h"

dap::Game::Game ( std::string projectDir ) {
	this->projectDir = projectDir;
}

dap::Game::~Game () {
	dap::Input::Clear ();
	dap::JSONobject::ClearFactories ();
	dap::Scene::ClearAll ();
}

void dap::Game::Close () {
	shouldClose = true;
}

std::string dap::Game::GetProjectDir () {
	return projectDir;
}

bool dap::Game::ShouldClose () {
	return shouldClose;
}
