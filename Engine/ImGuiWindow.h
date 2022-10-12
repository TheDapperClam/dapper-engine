#pragma once

#include "imgui.h"
#include <string>
#include <vector>

namespace dap {
	class ImGuiWindow {
	private:
		static const int MARGIN_X;
		static const int MARGIN_Y;

	private:
		static std::vector<dap::ImGuiWindow*> AllWindows;

	public:
		static void ClearAllWindows ();
		static const std::vector<dap::ImGuiWindow*>* GetAllWindows ();

	protected:
		ImGuiWindowFlags windowFlags = ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_AlwaysVerticalScrollbar;
		std::string title = "ImGuiWindow";

	private:
		bool shouldShow = false;

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
