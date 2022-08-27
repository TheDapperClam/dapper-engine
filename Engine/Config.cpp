#include "Config.h"

#include <filesystem>

const std::string dap::Config::PATH_DEFAULT = "Resources/Config/options.ini";

mINI::INIStructure dap::Config::properties;

std::string dap::Config::GetProperty ( std::string section, std::string key ) {
	if ( !properties.has ( section ) || !properties[section].has ( key ) )
		return "";
	return properties[section][key];
};

void dap::Config::Initialize () {
	// Default config options go here
	properties["rendering"]["bFullscreen"] = "true";
	properties["rendering"]["iWindowW"] = "800";
	properties["rendering"]["iWindowH"] = "600";
	properties["rendering"]["bVSync"] = "true";
}

void dap::Config::Load ( std::string path ) {
	if ( !std::filesystem::exists ( path ) )
		return;
	mINI::INIFile file ( path );
	mINI::INIStructure loadedIni;
	file.read ( loadedIni );
	for ( const auto& iniSection : loadedIni ) {
		const std::string SECTION = iniSection.first;
		const mINI::INIMap COLLECTION = iniSection.second;
		for ( const auto& iniCollection : COLLECTION ) {
			const std::string KEY = iniCollection.first;
			const std::string VALUE = iniCollection.second;
			SetProperty ( SECTION, KEY, VALUE );
		}
	}
}

void dap::Config::Save ( std::string path ) {
	std::filesystem::path fsp { path };
	std::filesystem::create_directories ( fsp.parent_path () );
	mINI::INIFile file ( path );
	if ( !std::filesystem::exists ( path ) )
		file.generate ( properties );
	else
		file.write ( properties );
}

void dap::Config::SetProperty ( std::string section, std::string key, std::string value ) {
	properties[section][key] = value;
}
