#pragma once

#include <ResourceImporter.h>
#include <string>
#include <vector>

namespace dap {
	class AudioImporter : public dap::ResourceImporter {
	public:
		void ClearImports () override;
		std::vector<std::string> GetExtensions () override;
		void Import ( std::string key, std::string path ) override;
	};
}
