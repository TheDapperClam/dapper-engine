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
		bool openSceneWindow = true;
		bool openCreateGameObjectPopup = false;
		dap::GameObject* selectedGameObject = 0;
		dap::FactoryMetaData* selectedAddComponentData = 0;
		std::string newGameObjectName;

	private:
		void Tick ();
	};
}
