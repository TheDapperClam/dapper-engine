#pragma once

#include <glad/glad.h>
#include <string>

namespace dap {
	class Texture {
	public:
		Texture () {};
		Texture ( const std::string fileName, const std::string name );
		~Texture ();
		const std::string* GetName ();
		void Use ();
	private:
		GLuint textureObject = 0;
		int width = 0;
		int height = 0;
		std::string name;
	};
}
