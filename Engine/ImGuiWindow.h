#pragma once

#include "imgui.h"
#include <string>
#include <vector>

namespace dap {
	class ImGuiWindow {
	private:
		static std::vector<dap::ImGuiWindow*> AllWindows;

	public:
		static void ClearAllWindows ();
		static const std::vector<dap::ImGuiWindow*>* GetAllWindows ();

	private:
		bool shouldShow = false;
		std::string title = "ImGuiWindow";

	public:
		ImGuiWindow ( std::string title );

	public:
		void Begin ();
		bool* const GetShow ();
		std::string GetTitle ();
		void SetShow ( bool show );

	protected:
		virtual void ContentBegin () = 0;
	};
};
