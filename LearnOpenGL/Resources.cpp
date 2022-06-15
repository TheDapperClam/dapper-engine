#include "Resources.h"
#include <algorithm>
#include <iostream>
#include <filesystem>
#include <string>

#include "TextureImporter.h"

const std::string dap::Resources::RESOURCES_ROOT = "./Resources";
const std::vector<dap::ResourceImporter*> dap::Resources::IMPORTERS = {
	new dap::TextureImporter ()
};
std::map<std::string, dap::ResourceImporter*> dap::Resources::extensionImporters = {};

dap::Resources::Resources () {
	for ( auto* importer : IMPORTERS ) {
		for ( auto extension : importer->GetExtensions () ) {
			extensionImporters.insert ( std::pair<std::string, dap::ResourceImporter*> ( extension, importer ) );
		}
	}
	Load ( RESOURCES_ROOT );
}

dap::Resources::~Resources () {
	for ( auto* importer : IMPORTERS ) {
		delete importer;
	}
}

void dap::Resources::Load ( const std::string directory ) {
	for ( auto& entry : std::filesystem::directory_iterator ( directory ) ) {
		if ( std::filesystem::is_directory ( entry ) ) {
			Load ( entry.path ().string () );
			continue;
		}
		std::string pathString = entry.path ().string ();
		std::replace(pathString.begin(), pathString.end(), '\\', '/');
		const std::string fullFileName = entry.path ().filename ().string ();
		const int extensionStart = fullFileName.find_last_of ( '.' );
		const std::string fileWithoutExtension = fullFileName.substr ( 0, extensionStart );
		const std::string fileExtension = fullFileName.substr ( extensionStart, fullFileName.size () - extensionStart );

		if ( !extensionImporters.contains ( fileExtension ) )
			continue;
		std::cout << "Loading: " + pathString << std::endl;
		extensionImporters[fileExtension]->Import ( fileWithoutExtension, pathString );
	}
}
