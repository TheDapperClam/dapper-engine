#include "Resources.h"
#include <algorithm>
#include <iostream>
#include <filesystem>
#include <string>

#include "ResourceObject.h"
#include "TextureImporter.h"
#include "FontImporter.h"

const std::string dap::Resources::RESOURCES_DIR = "Resources/";

const std::vector<dap::ResourceImporter*> dap::Resources::IMPORTERS = {
	new dap::TextureImporter (),
	new dap::FontImporter ()
};
std::map<std::string, dap::ResourceImporter*> dap::Resources::extensionImporterPairs = {};
std::map<std::string, dap::ResourceObject*> dap::Resources::allResources = {};

dap::Resources::Resources () {
	for ( auto* importer : IMPORTERS ) {
		for ( auto extension : importer->GetExtensions () ) {
			extensionImporterPairs.insert ( std::pair<std::string, dap::ResourceImporter*> ( extension, importer ) );
		}
	}
	Load ( RESOURCES_DIR );
}

dap::Resources::~Resources () {
	for ( auto* importer : IMPORTERS ) {
		delete importer;
	}
}

void dap::Resources::Add ( std::string key, dap::ResourceObject* resource ) {
	if ( !resource || allResources.contains ( key ) )
		return;
	std::cout << "Loading resource: " + key << std::endl;
	allResources.insert ( std::pair<std::string, dap::ResourceObject*> ( key, resource ) );
}

dap::ResourceObject* const dap::Resources::Get ( std::string key ) {
	if ( !allResources.contains ( key ) )
		return NULL;
	return allResources[key];
}

void dap::Resources::Load ( const std::string directory ) {
	for ( auto& entry : std::filesystem::directory_iterator ( directory ) ) {
		try {
			if ( std::filesystem::is_directory ( entry ) ) {
				Load ( entry.path ().string () );
				continue;
			}
			std::string pathString = entry.path ().string ();
			std::replace ( pathString.begin (), pathString.end (), '\\', '/' );
			const std::string fullFileName = entry.path ().filename ().string ();
			const int extensionStart = fullFileName.find_last_of ( '.' );
			const std::string fileWithoutExtension = fullFileName.substr ( 0, extensionStart );
			const std::string fileExtension = fullFileName.substr ( extensionStart, fullFileName.size () - extensionStart );

			if ( !extensionImporterPairs.contains ( fileExtension ) )
				continue;
			extensionImporterPairs[fileExtension]->Import ( fileWithoutExtension, pathString );
		} catch (int e) {
			std::cout << "Failed to load resource: " << entry.path () << std::endl;
		}
	}
}
