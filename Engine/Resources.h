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
		static const std::string DIRECTORIES_TO_CREATE[];

	private:
		static std::vector<dap::ResourceImporter*> AllImporters;
		static std::unordered_map<std::string, dap::ResourceImporter*> ExtensionImporterPairs;
		static bool FirstImport;

	private:
		static void LoadDirectory ( const std::string directory );

	public:
		static void Clear ( bool deleteImporters = false );
		static void Import ();

	private:
		Resources ();
	};
}
