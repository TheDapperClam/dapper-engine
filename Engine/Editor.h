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
		bool openCreateGameObjectPopup = false;
		bool openSceneWindow = true;
		bool shouldRunGame = false;
		bool shouldQuit = false;
		bool visualizeColliders = true;
		
		std::string selectedGameObjectName = "";
		dap::GameObject* selectedGameObject = 0;
		std::string newGameObjectName;
		std::string saveScenePath;

	public:
		void Quit ();
		void RefreshSelected ();
		bool ShouldRunGame ();
		bool ShouldQuit ();
		bool VisualizeColliders () const;

	private:
		void Tick ();
		void SetSelectedGameObject ( dap::GameObject* object );
		void ShowMenuBar ();
		void ShowSceneWindow ();
	};
}
