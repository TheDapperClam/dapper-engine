#pragma once

#include <string>

namespace dap {
	class GameObject;
	struct FactoryMetaData;

	class Editor {
		friend class Rendering;

	private:
		static dap::Editor* CurrentEditor;

	public:
		static dap::Editor* const GetCurrent ();
		static void SetCurrent ( dap::Editor* editor );

	private:
		bool shouldRunGame = false;
		bool shouldQuit = false;
		bool openSceneWindow = true;
		bool openCreateGameObjectPopup = false;
		std::string selectedGameObjectName = "";
		dap::GameObject* selectedGameObject = 0;
		std::string newGameObjectName;
		std::string saveScenePath;

	public:
		void Quit ();
		void RefreshSelected ();
		bool ShouldRunGame ();
		bool ShouldQuit ();

	private:
		void Tick ();
		void SetSelectedGameObject ( dap::GameObject* object );
		void ShowMenuBar ();
		void ShowSceneWindow ();
	};
}
