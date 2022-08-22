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
	properties["rendering"]["bFullscreen"] = "true";
	properties["rendering"]["iWindowW"] = "800";
	properties["rendering"]["iWindowH"] = "600";
	properties["rendering"]["bVSync"] = "true";
	properties["debug"]["bShowColliders"] = "false";
}

void dap::Config::Load ( std::string path ) {
	if ( !std::filesystem::exists ( path ) ) {
		Initialize ();
		return;
	}
	mINI::INIFile file ( path );
	file.read ( properties );
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
