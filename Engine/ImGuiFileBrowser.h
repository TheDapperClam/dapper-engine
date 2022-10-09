#pragma once

#include "ImGuiWindow.h"
#include <string>

namespace dap {
	class ImGuiFileBrowser : public dap::ImGuiWindow {
	private:
		std::string currentDirectory = "";
		std::string filename = "";

	public:
		std::string GetPath ();
		ImGuiFileBrowser ( std::string title );

	protected:
		virtual void ContentBegin () override;
	};
};
