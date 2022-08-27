#include "Game.h"

#include "Config.h"
#include "Editor.h"
#include "Input.h"
#include "JSONobject.h"
#include "Rendering.h"
#include "Resources.h"
#include "Scene.h"

dap::Game::~Game () {
	dap::Config::Save ( dap::Config::PATH_DEFAULT );
	dap::Input::Clear ();
	dap::JSONobject::ClearFactories ();
	dap::Scene::UnloadCurrent ();
}

void dap::Game::Close () {
	shouldClose = true;
}

void dap::Game::Run ( std::string name, unsigned int tickRate ) {
	dap::Editor editor;

	dap::Config::Initialize ();
	dap::Config::Load ( dap::Config::PATH_DEFAULT );
	dap::JSONobject::RegisterEngineObjects ();
	dap::Editor::SetCurrent ( &editor );
	dap::Rendering::Start ( name, tickRate, this );
}

bool dap::Game::ShouldClose () {
	return shouldClose;
}
