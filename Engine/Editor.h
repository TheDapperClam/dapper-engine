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
		dap::GameObject* selectedGameObject = 0;
		dap::FactoryMetaData* selectedAddComponentData = 0;
		std::string newGameObjectName;
		std::string saveScenePath;

	public:
		void ClearSelected ();
		void Quit ();
		bool ShouldRunGame ();
		bool ShouldQuit ();

	private:
		void Tick ();
		void ShowMenuBar ();
		void ShowSceneWindow ();
	};
}
