#include "Resources.h"

#include <algorithm>
#include <iostream>
#include <filesystem>
#include <string>
#include "TextureAtlas.h"
#include "TextureImporter.h"
#include "FontImporter.h"
#include "ResourceImporter.h"
#include "ShaderImporter.h"

const std::string dap::Resources::RESOURCES_DIR = "Resources/";

std::vector<dap::ResourceImporter*> dap::Resources::allImporters = {
	new dap::TextureImporter (),
	new dap::FontImporter (),
	new dap::ShaderImporter ()
};
std::map<std::string, dap::ResourceImporter*> dap::Resources::extensionImporterPairs = {};

dap::Resources::Resources () {
	for ( const auto& importer : allImporters ) {
		for ( const auto& extension : importer->GetExtensions () ) {
			extensionImporterPairs.insert ( std::pair<std::string, dap::ResourceImporter*> ( extension, importer ) );
		}
	}
	Load ( RESOURCES_DIR );

	dap::TextureAtlas::GetMainAtlas ()->Build ();

	for ( const auto& importer : allImporters ) {
		importer->ImportingComplete ();
	}
}

dap::Resources::~Resources () {
	for ( auto* importer : allImporters ) {
		importer->ClearImports ();
		delete importer;
	}
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
