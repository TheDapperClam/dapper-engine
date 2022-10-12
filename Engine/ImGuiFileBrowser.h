#pragma once

#include "Event.h"
#include "ImGuiWindow.h"
#include <string>
#include <vector>

namespace dap {
	class ImGuiFileBrowser : public dap::ImGuiWindow {
	public:
		enum class Mode {
			Save = 0,
			Open
		};

		enum class Response {
			None = 0,
			Cancel,
			OK
		};

	private:
		static const int DIRECTORY_DISPLAY_SIZE;

	public:
		dap::Event<dap::ImGuiFileBrowser::Response, std::string&> eventOnRespond;

	private:
		std::string currentDirectory = "";
		std::string filename = "";
		std::vector<std::string> filters = {};
		std::string filtersDisplayString = "";
		dap::ImGuiFileBrowser::Mode mode = dap::ImGuiFileBrowser::Mode::Save;

	public:
		ImGuiFileBrowser ( dap::ImGuiFileBrowser::Mode mode );

	public:
		dap::Event<dap::ImGuiFileBrowser::Response, std::string&>* const GetEventOnRespond ();
		dap::ImGuiFileBrowser::Mode GetMode ();
		void SetFilters ( std::vector<std::string> newFilters );
		void SetMode ( dap::ImGuiFileBrowser::Mode mode );

	private:
		void Respond ( dap::ImGuiFileBrowser::Response response );

	protected:
		virtual void ContentBegin () override;
	};
};
