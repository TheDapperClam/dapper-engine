#include "Game.h"

#include "Config.h"
#include "Input.h"
#include "JSONobject.h"
#include "Rendering.h"
#include "Resources.h"
#include "Scene.h"

dap::Game::~Game () {
	dap::Config::Save ( dap::Config::PATH_DEFAULT );
	dap::Scene::UnloadCurrent ();
	dap::Input::Clear ();
	dap::JSONobject::ClearFactories ();
}

std::string dap::Game::GetStartingScene () {
	return startingScene;
}

unsigned int dap::Game::GetTickRate () {
	return tickRate;
}

void dap::Game::Run ( std::string name, unsigned int tickRate, std::string startingScene ) {
	this->tickRate = tickRate;
	this->startingScene = startingScene;

	dap::Config::Initialize ();
	dap::Config::Load ( dap::Config::PATH_DEFAULT );
	dap::JSONobject::RegisterEngineObjects ();
	dap::Rendering::Start ( name, this );
}

bool dap::Game::ShouldStop () {
	return shouldStop;
}

void dap::Game::Stop () {
	shouldStop = true;
}
