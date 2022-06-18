#pragma once

#include <string>
#include <glad/glad.h>

namespace dap {
	class Shader {
	public:
		Shader ( std::string vertex, std::string fragment );
		~Shader ();

	public:
		const GLuint* GetProgram ();
		void Use ();

	private:
		GLuint program = -1;
	};
}
