#pragma once

#include "ResourceImporter.h"
#include <string>
#include <vector>

namespace dap {
	class Font;

	class FontImporter : public dap::ResourceImporter {
	private:
		std::vector<Font*> fontsToFree;

	public:
		virtual void ClearImports () override;
		virtual void Import ( std::string key, std::string path ) override;
		virtual void ImportingComplete () override;

	protected:
		virtual std::vector<std::string> GetExtensions () override;
	};
}
