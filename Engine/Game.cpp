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

dap::Game* dap::Game::Current = NULL;

dap::Game::~Game () {
	dap::Config::Save ( dap::Config::PATH_DEFAULT );
	dap::Scene::UnloadCurrent ();
	dap::Input::Clear ();
	dap::JSONObject::ClearFactories ();
}

void dap::Game::BaseEnd () {
	End ();
	for ( const auto& GAMEOBJECT : dap::Scene::GetCurrent ()->GetAllGameObjectsList () ) {
		for ( const auto& COMP : GAMEOBJECT->GetAllComponentsList () ) {
			COMP->BaseEnd ();
		}
	}
	dap::Scene::ClearPrefabCache ();
	running = false;
}

void dap::Game::BaseInput () {
	Input ();
	for ( const auto& GAMEOBJECT : dap::Scene::GetCurrent ()->GetAllGameObjectsList () ) {
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
	running = true;
	dap::Scene::ClearPrefabCache ();
	Start ();
	for ( const auto& GAMEOBJECT : dap::Scene::GetCurrent ()->GetAllGameObjectsList () ) {
		for ( const auto& COMP : GAMEOBJECT->GetAllComponentsList () ) {
			COMP->BaseStart ();
		}
	}
}

void dap::Game::BaseTick () {
	Tick ();
	for ( const auto& GAMEOBJECT : dap::Scene::GetCurrent ()->GetAllGameObjectsList () ) {
		for ( const auto& COMP : GAMEOBJECT->GetAllComponentsList () ) {
			COMP->BaseTick ();
		}
	}
	dap::Collision::Process ();
	dap::Scene::GetCurrent ()->DeleteMarkedObjects ();
	dap::Scene::BeginLateLoad ();
}

dap::Game* const dap::Game::GetCurrent () {
	return Current;
}

std::string dap::Game::GetStartingScene () {
	return startingScene;
}

unsigned int dap::Game::GetTickRate () {
	return tickRate;
}

bool dap::Game::IsRunning () {
	return running;
}

void dap::Game::Run ( std::string name, std::string startingScene, unsigned int tickRate ) {
	if ( Current && Current->IsRunning () ) {
		std::cout << "[GAME] Attempting to run game while another is already running" << std::endl;
		return;
	}
	Current = this;
	this->tickRate = tickRate;
	this->startingScene = startingScene;

	dap::Config::Initialize ();
	dap::Config::Load ( dap::Config::PATH_DEFAULT );
	BaseRegisterFactories ();
	dap::Rendering::Start ( name, this );
}

void dap::Game::Stop () {
	running = false;
}
