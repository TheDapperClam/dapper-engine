#pragma once

namespace dap {
	class GameObject;

	class Editor {
		friend class Rendering;

	private:
		static dap::Editor* CurrentEditor;

	public:
		static dap::Editor* const GetCurrent ();
		static void SetCurrent ( dap::Editor* editor );

	private:
		bool openSceneWindow = true;
		dap::GameObject* selectedGameObject = 0;

	private:
		void Tick ();
	};
}
