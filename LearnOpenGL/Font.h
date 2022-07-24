#pragma once

#include "ResourceObject.h"
#include <string>

namespace dap {
	class Font : public dap::ResourceObject {
	public:
		Font ( std::string path );
		bool BuildTexture ( std::string path );
	};
}
