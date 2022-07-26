#pragma once

#include <string>

namespace dap {
	struct TextureAtlasImageData {
		std::string name = "";
		int width = 0, height = 0;
		unsigned char* bytes = 0;
	};
}
