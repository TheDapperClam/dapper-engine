#pragma once

#include <unordered_map>
#include <string>
#include <vector>

namespace dap {
	class ResourceImporter;
	class ResourceObject;

	class Resources {
	private:
		static const std::string RESOURCES_DIR;
		static std::vector<dap::ResourceImporter*> allImporters;
		static std::unordered_map<std::string, dap::ResourceImporter*> extensionImporterPairs;

	private:
		static void Load ( const std::string directory );

	public:
		Resources ();
		~Resources ();
	};
}
