#pragma once
#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace dap {
	class Shader {
	public:
		Shader ( std::string vertex, std::string fragment );
		const GLuint* GetProgram ();
		void Use ();
		~Shader ();
	private:
		GLuint program = -1;
	};
}
