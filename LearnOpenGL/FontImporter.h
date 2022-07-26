#pragma once

#include "ResourceImporter.h"
#include <string>
#include <vector>

namespace dap {
	class FontImporter : public dap::ResourceImporter {
	public:
		virtual void Import ( std::string key, std::string path ) override;
		virtual void ImportingComplete () override;

	protected:
		virtual std::vector<std::string> GetExtensions () override;
	};
}
