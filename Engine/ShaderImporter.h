#pragma once

#include "ResourceImporter.h"
#include <string>
#include <vector>

namespace dap {
	class ShaderImporter : public dap::ResourceImporter {
	public:
		virtual void ClearImports () override;
		virtual void Import ( std::string key, std::string path ) override;
		virtual void ImportingComplete () override;
		virtual std::vector<std::string> GetExtensions () override;
	};
}
