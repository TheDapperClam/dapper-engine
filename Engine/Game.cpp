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
	const auto ALL_GAMEOJBECTS = dap::Scene::GetCurrent ()->GetAllGameObjectsList ();

	End ();
	for ( const auto& GAMEOBJECT : ALL_GAMEOJBECTS ) {
		for ( const auto& COMP : GAMEOBJECT->GetAllComponentsList () ) {
			COMP->BaseEnd ();
		}
	}
	dap::Scene::ClearPrefabCache ();
}

void dap::Game::BaseInput () {
	const auto ALL_GAMEOBJECTS = dap::Scene::GetCurrent ()->GetAllGameObjectsList ();

	Input ();
	for ( const auto& GAMEOBJECT : ALL_GAMEOBJECTS ) {
		for ( const auto& COMP : GAMEOBJECT->GetAllComponentsList () ) {
			COMP->BaseInput ();
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
	dap::Scene::ClearPrefabCache ();
	Start ();
	// We will start our components in BaseTick, more info there
}

void dap::Game::BaseTick () {
	const auto ALL_GAMEOBJECTS = dap::Scene::GetCurrent ()->GetAllGameObjectsList ();

	Tick ();
	for ( const auto& GAMEOBJECT : ALL_GAMEOBJECTS ) {
		for ( const auto& COMP : GAMEOBJECT->GetAllComponentsList () ) {
			// We start our components here so that any new ones spawned after game start get started
			COMP->BaseStart ();
			COMP->BaseTick ();
		}
	}
	dap::Collision::Process ();
	dap::Scene::GetCurrent ()->DeleteMarkedObjects ();
	dap::Scene::BeginLateLoad ();
}

std::string dap::Game::GetStartingScene () {
	return startingScene;
}

unsigned int dap::Game::GetTickRate () {
	return tickRate;
}

void dap::Game::Run ( std::string name, std::string startingScene, unsigned int tickRate ) {
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
