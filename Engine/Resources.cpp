#include "Resources.h"

#include <algorithm>
#include "Editor.h"
#include <filesystem>
#include <iostream>
#include "Scene.h"
#include <string>
#include "TextureAtlas.h"

#include "FontImporter.h"
#include "ResourceImporter.h"
#include "ShaderImporter.h"
#include "TextureImporter.h"
#include "TilesetImporter.h"

const std::string dap::Resources::RESOURCES_DIR = "Resources/";
const std::string dap::Resources::DIRECTORIES_TO_CREATE[] = {
	"Resources/Audio/",
	"Resources/Config/",
	"Resources/Fonts/",
	"Resources/Prefabs",
	"Resources/Scenes/",
	"Resources/Shaders/",
	"Resources/Textures/"
};

std::vector<dap::ResourceImporter*> dap::Resources::AllImporters = {
	new dap::TextureImporter (),
	new dap::FontImporter (),
	new dap::ShaderImporter (),
	new dap::TilesetImporter ()
};

std::unordered_map<std::string, dap::ResourceImporter*> dap::Resources::ExtensionImporterPairs = {};
bool dap::Resources::FirstImport = true;

dap::Resources::Resources () {

}

void dap::Resources::Clear ( bool deleteImporters ) {
	for ( auto* importer : AllImporters ) {
		importer->ClearImports ();
		if ( deleteImporters )
			delete importer;
	}
}

void dap::Resources::Import () {
	if ( !FirstImport )
		Clear ();
	for ( const auto& DIR : DIRECTORIES_TO_CREATE ) {
		if ( std::filesystem::exists ( DIR ) )
			continue;
		std::cout << "[RESOURCES] Creating directory: " << DIR << std::endl;
		std::filesystem::create_directory ( DIR );
	}

	for ( const auto& importer : AllImporters ) {
		for ( const auto& extension : importer->GetExtensions () ) {
			if ( ExtensionImporterPairs.contains ( extension ) )
				continue;
			ExtensionImporterPairs.insert ( std::pair<std::string, dap::ResourceImporter*> ( extension, importer ) );
		}
	}
	LoadDirectory ( RESOURCES_DIR );

	dap::TextureAtlas::GetMainAtlas ()->Build ();
	for ( const auto& importer : AllImporters ) {
		importer->TextureAtlasBuilt ();
		importer->ImportingComplete ();
	}
	dap::Editor* currentEditor = dap::Editor::GetCurrent ();
	if ( !FirstImport && currentEditor )
		currentEditor->ReloadEditorScene ();
	FirstImport = false;
}

void dap::Resources::LoadDirectory ( const std::string directory ) {
	for ( auto& entry : std::filesystem::directory_iterator ( directory ) ) {
		try {
			if ( std::filesystem::is_directory ( entry ) ) {
				LoadDirectory ( entry.path ().string () );
				continue;
			}
			std::string pathString = entry.path ().string ();
			std::replace ( pathString.begin (), pathString.end (), '\\', '/' );
			const std::string fullFileName = entry.path ().filename ().string ();
			const int extensionStart = fullFileName.find_last_of ( '.' );
			const std::string fileWithoutExtension = fullFileName.substr ( 0, extensionStart );
			const std::string fileExtension = fullFileName.substr ( extensionStart, fullFileName.size () - extensionStart );

			if ( !ExtensionImporterPairs.contains ( fileExtension ) )
				continue;
			std::cout << "[RESOURCES] Importing: " << pathString << std::endl;
			ExtensionImporterPairs[fileExtension]->Import ( fileWithoutExtension, pathString );
		} catch (int e) {
			std::cout << "[RESOURCES] Failed to load: " << entry.path () << std::endl;
		}
	}
}
