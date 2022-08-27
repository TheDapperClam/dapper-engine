#pragma once

#include "ResourceImporter.h"
#include <string>
#include <vector>

namespace dap {
	class TextureImporter : public dap::ResourceImporter {
	private:
		std::vector<unsigned char*> allImageData;

	public:
		virtual void ClearImports () override;
		virtual void Import ( std::string key, std::string path ) override;
		virtual void ImportingComplete () override;

	protected:
		virtual std::vector<std::string> GetExtensions () override;
	};
}
