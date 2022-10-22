#include "JSONObject.h"

#include <filesystem>
#include <fstream>

std::map<std::string, dap::FactoryMetaData*> dap::JSONObject::RegisteredFactories = {};

void dap::JSONObject::ClearFactories () {
	std::cout << "[JSONOBJECT] Clearing factories" << std::endl;
	for ( auto& pair : RegisteredFactories )
		delete RegisteredFactories[pair.first];
	RegisteredFactories.clear ();
}

std::string dap::JSONObject::GetFilePath () {
	return filePath;
}

dap::FactoryMetaData* const dap::JSONObject::GetMetaData ( const std::string typeID ) {
	if ( !RegisteredFactories.count ( typeID ) )
		return NULL;
	return RegisteredFactories[typeID];
}

const std::map<std::string, dap::FactoryMetaData*>* dap::JSONObject::GetRegisteredFactories () {
	return &RegisteredFactories;
}

void dap::JSONObject::Load ( std::string path ) {
	std::ifstream ifs ( path );
	if ( ifs.good () ) {
		nlohmann::json json = nlohmann::json::parse ( ifs );
		Deserialize ( &json );
	}
	ifs.close ();
	filePath = path;
}

void dap::JSONObject::Save ( std::string path ) {
	std::filesystem::path filesystemPath ( path );
	const std::string TEMP_FILE = filesystemPath.replace_filename ( "temp_" + filesystemPath.filename ().string () ).string ();
	std::ofstream ofs;

	if ( !std::filesystem::exists ( path ) )
		std::filesystem::create_directory ( std::filesystem::path ( path ).parent_path () );
	ofs.open ( TEMP_FILE );
	const bool OFS_GOOD = ofs.good ();
	if ( OFS_GOOD ) {
		std::cout << "[JSONOBJECT] Saving " << path << std::endl;
		nlohmann::json json = Serialize ();
		ofs << json;
	}
	ofs.close ();
	if ( OFS_GOOD )
		std::filesystem::rename ( TEMP_FILE, path );
	filePath = path;
}

nlohmann::json dap::JSONObject::Serialize () {
	nlohmann::json json;
	json["type"] = typeid( *this ).name ();
	return json;
}
