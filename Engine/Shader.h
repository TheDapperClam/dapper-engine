#pragma once

#include <glad/glad.h>
#include <string>
#include <map>

namespace dap {
	class Shader {
	private:
		static const std::string VERTEX_START_LABEL;
		static const std::string FRAGMENT_START_LABEL;
		static const std::string VERTEX_END_LABEL;
		static const std::string FRAGMENT_END_LABEL;

	private:
		static std::map<std::string, dap::Shader*> AllShaders;

	public:
		static void ClearShaders ();
		static std::map<std::string, dap::Shader*>* const GetAllShaders ();
		static dap::Shader* const GetShader ( std::string key );

	private:
		GLuint program = -1;
		std::string name;
		bool eraseOnDelete = true;

	public:
		Shader ( std::string name, std::string path );
		~Shader ();

	public:
		const GLuint* GetProgram ();
		std::string GetName ();
		void Use ();
	};
}
