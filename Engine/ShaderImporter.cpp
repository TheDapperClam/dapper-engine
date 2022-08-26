#include "ShaderImporter.h"
#include "Resources.h"
#include "Shader.h"

std::vector<std::string> dap::ShaderImporter::GetExtensions () {
	return { ".shader" };
}

void dap::ShaderImporter::Import ( std::string key, std::string path ) {
	dap::Shader* shader = new dap::Shader ( key, path );
	dap::Resources::Add ( key, shader );
}

void dap::ShaderImporter::ImportingComplete () {
	
}
