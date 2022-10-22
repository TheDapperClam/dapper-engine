#pragma once

#include "ImGuiFileBrowser.h"
#include "ImGuiWindow.h"
#include <string>
#include <vector>

namespace dap {
	class GameObject;

	class ImGuiGameObjectEditor : public dap::ImGuiWindow {
	private:
		static const std::vector<dap::GameObject*>* ALL_GAMEOBJECTS;

	private:
		dap::ImGuiFileBrowser* fileBrowser = NULL;
		std::string loadPrefabPath = "";
		std::string newGameObjectName = "";
		bool openCreateGameObjectPopup = false;
		std::string savePrefabPath = "";
		std::string selectedGameObjectName = "";
		dap::GameObject* selectedGameObject = 0;

	public:
		ImGuiGameObjectEditor ( std::string title );

	public:
		dap::GameObject* GetSelectedGameObject ();
		std::string GetSelectedGameObjectName ();
		void SetSelectedGameObject ( dap::GameObject* object );

	private:
		void OnFileBrowserRespond ( dap::ImGuiFileBrowser::Response response, std::string& path );
		void ShowCreateGameObjectPopup ();

	protected:
		virtual void ContentBegin () override;
	};
};
