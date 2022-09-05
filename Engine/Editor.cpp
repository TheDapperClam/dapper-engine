#include "Editor.h"

#include "Component.h"
#include "Config.h"
#include "FactoryMetaData.h"
#include "GameObject.h"
#include "imgui.h"
#include "misc/cpp/imgui_stdlib.h"
#include <iostream>
#include "JSONobject.h"
#include "Rendering.h"
#include "Scene.h"

dap::Editor* dap::Editor::CurrentEditor = NULL;

dap::Editor* const dap::Editor::GetCurrent () {
	return CurrentEditor;
}

void dap::Editor::Quit () {
	shouldQuit = true;
}

void dap::Editor::RefreshSelected () {
	dap::GameObject* obj = dap::Scene::GetCurrent ()->GetGameObject ( selectedGameObjectName );
	SetSelectedGameObject ( obj );
}

void dap::Editor::SetCurrent ( dap::Editor* editor ) {
	CurrentEditor = editor;
}

void dap::Editor::SetSelectedGameObject ( dap::GameObject* object ) {
	selectedGameObject = object;
	selectedGameObjectName = object ? object->GetName () : "";
}

bool dap::Editor::ShouldRunGame () {
	return shouldRunGame;
}

void dap::Editor::ShowCreateGameObjectPopup () {
	dap::Scene* currentScene = dap::Scene::GetCurrent ();

	ImGui::OpenPopup ( "Create GameObject" );
	if ( ImGui::BeginPopupModal ( "Create GameObject", &openCreateGameObjectPopup, ImGuiWindowFlags_AlwaysAutoResize ) ) {
		ImGui::InputText ( "Name", &newGameObjectName );

		const bool NAME_ENTERED = newGameObjectName.find_first_not_of ( ' ' ) != std::string::npos;
		const bool NAME_TAKEN = currentScene->GetGameObject ( newGameObjectName );
		const bool NAME_VALID = NAME_ENTERED && !NAME_TAKEN;

		if ( !NAME_ENTERED )
			ImGui::Text ( "Please enter a name" );
		if ( NAME_TAKEN )
			ImGui::Text ( "GameObject with name already exists" );

		if ( NAME_VALID ) {
			if ( ImGui::Button ( "Create" ) ) {
				SetSelectedGameObject ( currentScene->SpawnGameObject ( newGameObjectName ) );
				openCreateGameObjectPopup = false;
				ImGui::CloseCurrentPopup ();
			}
			ImGui::SameLine ();
		}

		if ( ImGui::Button ( "Cancel" ) ) {
			openCreateGameObjectPopup = false;
			ImGui::CloseCurrentPopup ();
		}
		ImGui::EndPopup ();
	}
}

void dap::Editor::ShowMenuBar () {
	dap::Scene* currentScene = dap::Scene::GetCurrent ();

	if ( !ImGui::BeginMainMenuBar () ) {
		ImGui::EndMainMenuBar ();
		return;
	}

	if ( ImGui::BeginMenu ( "File" ) ) {
		if ( !shouldRunGame ) {
			if ( ImGui::MenuItem ( "Save" ) )
				currentScene->Save ( currentScene->GetPath () );
			ImGui::Separator ();
		}
		if ( selectedGameObject ) {
			if ( ImGui::MenuItem ( "Save Prefab" ) )
				openSavePrefabPopup = true;
			ImGui::Separator ();
		}
		if ( ImGui::MenuItem ( "Quit" ) )
			Quit ();
		ImGui::EndMenu ();
	}

	if ( ImGui::BeginMenu ( "View" ) ) {
		bool isFullscreen = dap::Config::GetProperty ( "rendering", "bFullscreen" ) == "true";
		if ( ImGui::Checkbox ( "Fullscreen", &isFullscreen ) ) {
			dap::Config::SetProperty ( "rendering", "bFullscreen", isFullscreen ? "true" : "false" );
			dap::Rendering::UpdateWindowProperties ();
		}
		ImGui::Separator ();
		ImGui::Checkbox ( "Scene Window", &openSceneWindow );
		ImGui::Separator ();
		ImGui::Checkbox ( "Visualize Colliders", &visualizeColliders );
		ImGui::EndMenu ();
	}

	if ( !shouldRunGame ) {
		if ( ImGui::Button ( "Play" ) ) {
			currentScene->Save ( currentScene->GetPath () );
			shouldRunGame = true;
		}
	} else {
		if ( ImGui::Button ( "Stop" ) )
			shouldRunGame = false;
	}

	ImGui::EndMainMenuBar ();
}

void dap::Editor::ShowSavePrefabPopup () {
	const std::string TITLE = "Save Prefab - " + selectedGameObject->GetName ();
	ImGui::OpenPopup ( TITLE.c_str () );
	if ( ImGui::BeginPopupModal ( TITLE.c_str (), &openSavePrefabPopup, ImGuiWindowFlags_AlwaysAutoResize ) ) {
		ImGui::InputText ( "Path", &savePrefabPath );
		const bool PATH_ENTERED = savePrefabPath.find_first_not_of ( ' ' ) != std::string::npos;

		if ( PATH_ENTERED ) {
			if ( ImGui::Button ( "Save" ) ) {
				selectedGameObject->SavePrefab ( savePrefabPath );
				openSavePrefabPopup = false;
				ImGui::CloseCurrentPopup ();
			}
			ImGui::SameLine ();
		} else
			ImGui::Text ( "Please enter a save path" );
		if ( ImGui::Button ( "Cancel" ) ) {
			openSavePrefabPopup = false;
			ImGui::CloseCurrentPopup ();
		}

		ImGui::EndPopup ();
	}
}

void dap::Editor::ShowSceneWindow () {
	dap::Scene* currentScene = dap::Scene::GetCurrent ();
	if ( !currentScene )
		return;

	if ( !openSceneWindow )
		return;
	if ( !ImGui::Begin ( ( "Scene - " + currentScene->GetName () ).c_str (), &openSceneWindow, ImGuiWindowFlags_AlwaysAutoResize ) ) {
		ImGui::End ();
		return;
	}

	if ( ImGui::Button ( "Create Object" ) )
		openCreateGameObjectPopup = true;

	if ( selectedGameObject ) {
		ImGui::SameLine ();
		if ( ImGui::Button ( "Delete Object" ) ) {
			delete selectedGameObject;
			SetSelectedGameObject ( NULL );
		}
	}

	if ( ImGui::BeginCombo ( "Selected Object", selectedGameObject ? selectedGameObject->GetName ().c_str () : "" ) ) {
		for ( const auto& GAMEOBJECT : currentScene->GetAllGameObjectsList () ) {
			bool isSelected = GAMEOBJECT == selectedGameObject;
			if ( ImGui::Selectable ( GAMEOBJECT->GetName ().c_str (), isSelected ) )
				SetSelectedGameObject ( GAMEOBJECT );
		}
		ImGui::EndCombo ();
	}

	if ( selectedGameObject ) {
		ImGui::Separator ();
		if ( ImGui::BeginCombo ( "##AddComponentCombo", "Add Component" ) ) {
			for ( const auto& jsonPair : *dap::JSONObject::GetRegisteredFactories () ) {
				if ( ImGui::Selectable ( jsonPair.second->Name.c_str (), false ) ) {
					dap::Component* const newComp = static_cast<dap::Component*>( dap::JSONObject::Factory ( jsonPair.second->TypeID ) );
					selectedGameObject->AddComponent ( newComp );
				}
			}
			ImGui::EndCombo ();
		}
		ImGui::Separator ();
		selectedGameObject->ImGuiWidgetsBegin ();
	}

	ImGui::End ();
}

bool dap::Editor::ShouldQuit () {
	return shouldQuit;
}

void dap::Editor::Tick () {
	ShowMenuBar ();
	ShowSceneWindow ();
	if ( openCreateGameObjectPopup )
		ShowCreateGameObjectPopup ();
	if ( openSavePrefabPopup )
		ShowSavePrefabPopup ();
}

bool dap::Editor::VisualizeColliders () const {
	return visualizeColliders;
}
