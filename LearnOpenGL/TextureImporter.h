#pragma once

#include "ResourceImporter.h"

namespace dap {
	class TextureImporter : public dap::ResourceImporter {
	public:
		virtual void Import ( std::string key, std::string path ) const override;

	protected:
		virtual std::vector<std::string> GetExtensions () const override;
	};
}
