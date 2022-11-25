#include "ShaderImporter.h"

#include <iostream>
#include "Shader.h"

void dap::ShaderImporter::ClearImports () {
	dap::Shader::ClearShaders ();
}

std::vector<std::string> dap::ShaderImporter::GetExtensions () {
	return { ".shader" };
}

void dap::ShaderImporter::Import ( std::string key, std::string path ) {
	dap::Shader* shader = new dap::Shader ( key, path );
}
