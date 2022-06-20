#pragma once

#include <glad/glad.h>
#include <string>

namespace dap {
	class Texture {
	private:
		static dap::Texture* currentBoundTexture;

	public:
		Texture () {};
		Texture ( const std::string fileName, const std::string name );
		~Texture ();

	public:
		const std::string* GetName ();
		void Use ();

	private:
		GLuint textureObject = 0;
		int width = 0;
		int height = 0;
		std::string name;
	};
}
