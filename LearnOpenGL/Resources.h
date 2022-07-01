#pragma once

#include <map>
#include <string>
#include <vector>

namespace dap {
	class ResourceImporter;
	class ResourceObject;

	class Resources {
	private:
		static const std::string RESOURCES_DIR;
		static std::vector<dap::ResourceImporter*> allImporters;
		static std::map<std::string, dap::ResourceImporter*> extensionImporterPairs;
		static std::map<std::string, dap::ResourceObject*> allResources;

	public:
		static void Add ( std::string key, dap::ResourceObject* resource );
		static dap::ResourceObject* const Get ( std::string key );

	private:
		static void Load ( const std::string directory );

	public:
		Resources ();
		~Resources ();
	};
}
