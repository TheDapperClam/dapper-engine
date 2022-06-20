#pragma once

#include <vector>
#include <string>
#include <map>

namespace dap {
	class ResourceImporter;

	class Resources {
	private:
		static const std::string RESOURCES_DIR;
		static const std::vector<dap::ResourceImporter*> IMPORTERS;
		static std::map<std::string, dap::ResourceImporter*> extensionImporterPairs;

	private:
		static void Load ( const std::string directory );

	public:
		Resources ();
		~Resources ();
	};
}
