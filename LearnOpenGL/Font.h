#pragma once

#include "ResourceObject.h"
#include <string>

namespace dap {
	class Font : public dap::ResourceObject {
	private:
		static const std::string GLYPHS_TO_LOAD;

	public:
		Font ( std::string path );
		bool BuildTexture ( std::string path );
	};
}
