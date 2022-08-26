#pragma once

#include <string>
#include <glad/glad.h>
#include "ResourceObject.h"

namespace dap {
	class Shader : public dap::ResourceObject {
	private:
		static const std::string VERTEX_START_LABEL;
		static const std::string FRAGMENT_START_LABEL;
		static const std::string VERTEX_END_LABEL;
		static const std::string FRAGMENT_END_LABEL;

	private:
		GLuint program = -1;
		std::string name;

	public:
		Shader ( std::string name, std::string path );
		~Shader ();

	public:
		const GLuint* GetProgram ();
		std::string GetName ();
		void Use ();
	};
}
