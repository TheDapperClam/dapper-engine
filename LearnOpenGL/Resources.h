#pragma once

#include <vector>
#include <string>
#include <map>

namespace dap {
	class ResourceImporter;

	class Resources {
	public:
		Resources ();
		~Resources ();
	private:
		static const std::string RESOURCES_ROOT;
		static const std::vector<dap::ResourceImporter*> IMPORTERS;
		static std::map<std::string, dap::ResourceImporter*> extensionImporters;

		static void Load ( const std::string directory );
	};
}
