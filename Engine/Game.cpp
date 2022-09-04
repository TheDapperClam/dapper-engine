#include "Game.h"

#include "Collision.h"
#include "Config.h"
#include "GameObject.h"
#include "Input.h"
#include "JSONobject.h"
#include "Rendering.h"
#include "Resources.h"
#include "Scene.h"

// Object factories to be registered
#include "Camera.h"
#include "Label.h"
#include "RectCollider.h"
#include "Sprite.h"
#include "Transform.h"

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
			for ( const auto& COMP : GAMEOBJECT_PAIR.second->GetAllComponentsList () ) {
				COMP->BaseEnd ();
			}
		}
	}
}

void dap::Game::BaseInput () {
	Input ();
	dap::Scene* scene = dap::Scene::GetCurrent ();
	if ( scene ) {
		for ( const auto& GAMEOBJECT_PAIR : *scene->GetAllGameObjects () ) {
			for ( const auto& COMP : GAMEOBJECT_PAIR.second->GetAllComponentsList () ) {
				COMP->BaseInput ();
			}
		}
	}
}

void dap::Game::BaseRegisterFactories () {
	dap::JSONObject::RegisterFactory<dap::Transform> ( new dap::FactoryMetaData ( "Transform", true ) );
	dap::JSONObject::RegisterFactory<dap::Camera> ( new dap::FactoryMetaData ( "Camera", true ) );
	dap::JSONObject::RegisterFactory<dap::Sprite> ( new dap::FactoryMetaData ( "Sprite", true ) );
	dap::JSONObject::RegisterFactory<dap::RectCollider> ( new dap::FactoryMetaData ( "RectCollider", true ) );
	dap::JSONObject::RegisterFactory<dap::Label> ( new dap::FactoryMetaData ( "Label", true ) );

	RegisterFactories ();
}

void dap::Game::BaseStart () {
	Start ();
	// We will start our components in BaseTick, more info there
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
			for ( const auto& COMP : gameObject->GetAllComponentsList () ) {
				// We start our components here so that any new ones spawned after game start get started
				COMP->BaseStart ();
				COMP->BaseTick ();
			}
		}
	}
	dap::Collision::Process ();
	for ( auto& GAMEOBJECT : objectsToDelete )
		delete GAMEOBJECT;
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
	BaseRegisterFactories ();
	dap::Rendering::Start ( name, this );
}

bool dap::Game::ShouldStop () {
	return shouldStop;
}

void dap::Game::Stop () {
	shouldStop = true;
}
