#include "Editor.h"

#include "Component.h"
#include "FactoryMetaData.h"
#include "GameObject.h"
#include "imgui.h"
#include "misc/cpp/imgui_stdlib.h"
#include <iostream>
#include "JSONobject.h"
#include "Scene.h"

dap::Editor* dap::Editor::CurrentEditor = NULL;

dap::Editor* const dap::Editor::GetCurrent () {
	return CurrentEditor;
}

void dap::Editor::SetCurrent ( dap::Editor* editor ) {
	CurrentEditor = editor;
}

void dap::Editor::Tick () {
	dap::Scene* currentScene = dap::Scene::GetCurrent ();
	if ( !currentScene )
		return;

	if ( !openSceneWindow )
		return;
	if ( !ImGui::Begin ( ( "Scene - " + currentScene->GetName () ).c_str (), &openSceneWindow, ImGuiWindowFlags_AlwaysAutoResize ) ) {
		ImGui::End ();
		return;
	}

	if ( ImGui::Button ( "Save" ) )
		currentScene->Save ( currentScene->GetPath () );

	ImGui::Separator ();
	if ( ImGui::Button ( "Create Object" ) ) {
		openCreateGameObjectPopup = true;
		ImGui::OpenPopup ( "Create GameObject" );
	}
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
				selectedGameObject = currentScene->SpawnGameObject ( newGameObjectName );
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

	if ( selectedGameObject ) {
		ImGui::SameLine ();
		if ( ImGui::Button ( "Delete Object" ) ) {
			selectedGameObject->Destroy ();
			selectedGameObject = NULL;
		}
	}

	if ( ImGui::BeginCombo ( "Selected Object", selectedGameObject ? selectedGameObject->GetName ().c_str () : "" ) ) {
		for ( const auto& gameObjPair : *currentScene->GetAllGameObjects () ) {
			dap::GameObject* obj = gameObjPair.second;
			bool isSelected = obj == selectedGameObject;
			if ( ImGui::Selectable ( obj->GetName ().c_str (), isSelected ) )
				selectedGameObject = obj;
		}
		ImGui::EndCombo ();
	}

	if ( selectedGameObject ) {
		ImGui::Separator ();
		if ( ImGui::BeginCombo ( "##label", selectedAddComponentData ? selectedAddComponentData->Name.c_str () : "" ) ) {
			for ( const auto& jsonPair : *dap::JSONobject::GetRegisteredFactories () ) {
				bool isSelected = jsonPair.second == selectedAddComponentData;
				if ( ImGui::Selectable ( jsonPair.second->Name.c_str (), isSelected ) )
					selectedAddComponentData = jsonPair.second;
			}
			ImGui::EndCombo ();
		}
		ImGui::SameLine ();
		if ( ImGui::Button ( "Add Component" ) && selectedAddComponentData ) {
			dap::Component* const newComp = static_cast<dap::Component*>( dap::JSONobject::Factory ( selectedAddComponentData->TypeID ) );
			selectedGameObject->AddComponent ( newComp );
			selectedAddComponentData = NULL;
		}
		ImGui::Separator ();
		selectedGameObject->ImGuiWidgetsBegin ();
	}

	ImGui::End ();
}
