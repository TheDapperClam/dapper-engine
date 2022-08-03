#pragma once

#include <string>

namespace dap {
	class Font;

	struct TextureGenData {
		std::string name = "";
		unsigned int width = 0, height = 0;
		unsigned char* bytes = 0;
		unsigned int pixelMode = 0;
		dap::Font* font = NULL;
	};
}
