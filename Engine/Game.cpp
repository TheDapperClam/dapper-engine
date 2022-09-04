#include "Game.h"

#include "Config.h"
#include "GameObject.h"
#include "Input.h"
#include "JSONobject.h"
#include "Rendering.h"
#include "Resources.h"
#include "Scene.h"

dap::Game::~Game () {
	dap::Config::Save ( dap::Config::PATH_DEFAULT );
	dap::Scene::UnloadCurrent ();
	dap::Input::Clear ();
	dap::JSONObject::ClearFactories ();
}

void dap::Game::BaseEnd () {
	shouldStop = false;
	dap::Scene::GetCurrent ()->ClearPrefabCache ();
	End ();
	dap::Scene* scene = dap::Scene::GetCurrent ();
	if ( scene ) {
		for ( const auto& GAMEOBJECT_PAIR : *scene->GetAllGameObjects () ) {
			for ( const auto& CONTROLLER : *GAMEOBJECT_PAIR.second->GetAllControllers () ) {
				CONTROLLER->BaseEnd ();
			}
		}
	}
}

void dap::Game::BaseInput () {
	Input ();
	dap::Scene* scene = dap::Scene::GetCurrent ();
	if ( scene ) {
		for ( const auto& GAMEOBJECT_PAIR : *scene->GetAllGameObjects () ) {
			for ( const auto& CONTROLLER : *GAMEOBJECT_PAIR.second->GetAllControllers () ) {
				CONTROLLER->BaseInput ();
			}
		}
	}
}

void dap::Game::BaseStart () {
	Start ();
	// We will start our controllers in BaseTick, more info there
}

void dap::Game::BaseTick () {
	Tick ();
	dap::Scene* scene = dap::Scene::GetCurrent ();
	std::vector<dap::GameObject*> objectsToDelete;
	if ( scene ) {
		for ( const auto& GAMEOBJECT_PAIR : *scene->GetAllGameObjects () ) {
			dap::GameObject* gameObject = GAMEOBJECT_PAIR.second;
			if ( gameObject->ShouldBeDestroyed () )
				objectsToDelete.push_back ( gameObject );
			for ( const auto& CONTROLLER : *gameObject->GetAllControllers () ) {
				// We start our controller here so that any new ones spawned after game start get started
				CONTROLLER->BaseStart ();
				CONTROLLER->BaseTick ();
			}
		}
		for ( auto& gameObject : objectsToDelete )
			scene->DeleteGameObject ( gameObject );
	}
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
	dap::JSONObject::RegisterEngineObjects ();
	dap::Rendering::Start ( name, this );
}

bool dap::Game::ShouldStop () {
	return shouldStop;
}

void dap::Game::Stop () {
	shouldStop = true;
}
