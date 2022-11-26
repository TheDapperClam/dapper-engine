#include "ImGuiGameObjectEditor.h"

#include <filesystem>
#include "GameObject.h"
#include "glm/glm.hpp"
#include "imgui_stdlib.h"
#include "Scene.h"

// TODO: remove?
#include "Camera.h"

// TODO: remove
#include "GLFW/glfw3.h"
#include "glm/gtx/string_cast.hpp"
#include <iostream>
#include "Input.h"
#include "Tilemap.h"

const std::vector<dap::GameObject*>* dap::ImGuiGameObjectEditor::ALL_GAMEOBJECTS = dap::Scene::GetAllGameObjects ();

dap::ImGuiGameObjectEditor::ImGuiGameObjectEditor ( std::string title ) : dap::ImGuiWindow ( title ) {
	windowFlags |= ImGuiWindowFlags_MenuBar;
	fileBrowser = new dap::ImGuiFileBrowser ( dap::ImGuiFileBrowser::Mode::Open );
	fileBrowser->SetFilters ( { dap::GameObject::GetFileExtension () } );
	fileBrowser->GetEventOnRespond ()->Subscribe<dap::ImGuiGameObjectEditor> ( &dap::ImGuiGameObjectEditor::OnFileBrowserRespond, this );
}

void dap::ImGuiGameObjectEditor::ContentBegin () {
	if ( ImGui::BeginMenuBar () ) {
		if ( ImGui::BeginMenu ( "File" ) ) {
			if ( selectedGameObject ) {
				if ( ImGui::MenuItem ( "Save Prefab" ) ) {
					const std::string PREFAB_PATH = selectedGameObject->GetFilePath ();
					if ( PREFAB_PATH.empty () ) {
						fileBrowser->SetMode ( dap::ImGuiFileBrowser::Mode::Save );
						fileBrowser->SetShow ( true );
					} else
						selectedGameObject->SaveFile ( PREFAB_PATH );
				}
				if ( ImGui::MenuItem ( "Save Prefab As" ) ) {
					fileBrowser->SetMode ( dap::ImGuiFileBrowser::Mode::Save );
					fileBrowser->SetShow ( true );
				}
				ImGui::Separator ();
			}
			if ( ImGui::MenuItem ( "Load Prefab" ) ) {
				fileBrowser->SetMode ( dap::ImGuiFileBrowser::Mode::Open );
				fileBrowser->SetShow ( true );
			}
			ImGui::EndMenu ();

		}
		ImGui::EndMenuBar ();
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
		for ( int i = 0; i < ALL_GAMEOBJECTS->size (); i++ ) {
			dap::GameObject* currentObject = ALL_GAMEOBJECTS->at ( i );
			bool isSelected = currentObject == selectedGameObject;
			if ( ImGui::Selectable ( currentObject->GetName ().c_str (), isSelected ) )
				SetSelectedGameObject ( currentObject );
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

	ShowCreateGameObjectPopup ();
}

dap::GameObject* dap::ImGuiGameObjectEditor::GetSelectedGameObject () {
	return selectedGameObject;
}

std::string dap::ImGuiGameObjectEditor::GetSelectedGameObjectName () {
	return selectedGameObjectName;
}

void dap::ImGuiGameObjectEditor::OnFileBrowserRespond ( dap::ImGuiFileBrowser::Response response, std::string& path ) {
	if ( response != dap::ImGuiFileBrowser::Response::OK )
		return;
	switch ( fileBrowser->GetMode () ) {
		case dap::ImGuiFileBrowser::Mode::Save:
			selectedGameObject->SaveFile ( path );
			break;
		case dap::ImGuiFileBrowser::Mode::Open:
			SetSelectedGameObject ( dap::Scene::GetCurrent ()->SpawnPrefab ( path ) );
			break;
	}
}

void dap::ImGuiGameObjectEditor::SetSelectedGameObject ( dap::GameObject* object ) {
	selectedGameObject = object;
	selectedGameObjectName = object ? object->GetName () : "";
	savePrefabPath = object ? object->GetFilePath () : "";
}

void dap::ImGuiGameObjectEditor::ShowCreateGameObjectPopup () {
	if ( !openCreateGameObjectPopup )
		return;
	dap::Scene* currentScene = dap::Scene::GetCurrent ();
	ImGui::OpenPopup ( "Create GameObject" );
	if ( ImGui::BeginPopupModal ( "Create GameObject", &openCreateGameObjectPopup, ImGuiWindowFlags_AlwaysAutoResize ) ) {
		ImGui::InputText ( "Name", &newGameObjectName );

		const bool NAME_ENTERED = newGameObjectName.find_first_not_of ( ' ' ) != std::string::npos;
		const bool NAME_TAKEN = currentScene->GetGameObject ( newGameObjectName );

		if ( NAME_ENTERED ) {
			if ( NAME_TAKEN )
				ImGui::Text ( "Name taken, GameObject ID will be appended to end" );
			if ( ImGui::Button ( "Create" ) ) {
				SetSelectedGameObject ( currentScene->SpawnGameObject ( newGameObjectName ) );
				openCreateGameObjectPopup = false;
				newGameObjectName = "";
				ImGui::CloseCurrentPopup ();
			}
			ImGui::SameLine ();
		} else {
			ImGui::Text ( "Please enter a name" );
		}

		if ( ImGui::Button ( "Cancel" ) ) {
			openCreateGameObjectPopup = false;
			ImGui::CloseCurrentPopup ();
		}
		ImGui::EndPopup ();
	}
}
