#include "Game.h"

#include "Audio.h"
#include "Collision.h"
#include "Component.h"
#include "Config.h"
#include "Editor.h"
#include "GameObject.h"
#include "ImGuiWindow.h"
#include "Input.h"
#include "JSONobject.h"
#include "Rendering.h"
#include "Resources.h"
#include "Scene.h"
#include "Tileset.h"

// Object factories to be registered
#include "Camera.h"
#include "Label.h"
#include "KinematicBody.h"
#include "RectCollider.h"
#include "Sprite.h"
#include "Tilemap.h"
#include "Transform.h"

const std::vector<dap::Component*>* dap::Game::COMPONENTS_TO_PROCESS = dap::Component::GetComponentsToProcess ();

dap::Game* dap::Game::Current = NULL;

dap::Game::~Game () {
	dap::Editor* currentEditor = dap::Editor::GetCurrent ();
	if ( currentEditor )
		currentEditor->ClearTemp ();
	dap::Config::Save ( dap::Config::PATH_DEFAULT );
	dap::Scene::UnloadCurrent ();
	dap::Input::DeleteInputs ();
	dap::ImGuiWindow::ClearAllWindows ();
	dap::JSONObject::ClearFactories ();
	dap::Tileset::ClearAllTilesets ();
	dap::Audio::Stop ();
}

void dap::Game::BaseEnd () {
	End ();
	for ( int i = 0; i < COMPONENTS_TO_PROCESS->size (); i++ ) {
		COMPONENTS_TO_PROCESS->at ( i )->BaseEnd ();
	}
	dap::Scene::ClearPrefabCache ();
	running = false;
}

void dap::Game::BaseInput () {
	Input ();
	for ( int i = 0; i < COMPONENTS_TO_PROCESS->size (); i++ ) {
		COMPONENTS_TO_PROCESS->at ( i )->BaseInput ();
	}
}

void dap::Game::BaseRegisterFactories () {
	dap::JSONObject::RegisterFactory<dap::Camera> ( new dap::FactoryMetaData ( "Camera", true ) );
	dap::JSONObject::RegisterFactory<dap::Label> ( new dap::FactoryMetaData ( "Label", true ) );
	dap::JSONObject::RegisterFactory<dap::KinematicBody> ( new dap::FactoryMetaData ( "KinematicBody", true ) );
	dap::JSONObject::RegisterFactory<dap::RectCollider> ( new dap::FactoryMetaData ( "RectCollider", true ) );
	dap::JSONObject::RegisterFactory<dap::Sprite> ( new dap::FactoryMetaData ( "Sprite", true ) );
	dap::JSONObject::RegisterFactory<dap::Tilemap> ( new dap::FactoryMetaData ( "Tilemap", true ) );
	dap::JSONObject::RegisterFactory<dap::Transform> ( new dap::FactoryMetaData ( "Transform", true ) );

	RegisterFactories ();
}

void dap::Game::BaseStart () {
	running = true;
	dap::Scene::ClearPrefabCache ();
	Start ();
	for ( int i = 0; i < COMPONENTS_TO_PROCESS->size (); i++ ) {
		COMPONENTS_TO_PROCESS->at ( i )->BaseStart ();
	}
}

void dap::Game::BaseTick () {
	Tick ();
	for ( int i = 0; i < COMPONENTS_TO_PROCESS->size (); i++ ) {
		COMPONENTS_TO_PROCESS->at ( i )->BaseTick ();
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
	dap::Audio::Start ();
	dap::Rendering::Start ( name, this );
}

void dap::Game::Stop () {
	running = false;
}
