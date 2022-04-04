#include "Shader.h"

#include <GLFW/glfw3.h>
#include <fstream>
#include <iostream>
#include <vector>

dap::Shader::Shader ( std::string vertex, std::string fragment ) {
	std::ifstream ifs;
	std::string vertexShaderSource;
	std::string fragmentShaderSource;

	ifs.open ( vertex, std::ifstream::in );
	while ( ifs.good () ) {
		char c = ifs.get ();
		if ( c >= 128 || c < 0 )
			continue;
		vertexShaderSource += c;
	}
	ifs.close ();

	ifs.open ( fragment, std::ifstream::in );
	while ( ifs.good () ) {
		char c = ifs.get ();
		if ( c >= 128 || c < 0 )
			continue;
		fragmentShaderSource += c;
	}
	ifs.close ();

	GLuint vertexShader = glCreateShader ( GL_VERTEX_SHADER );
	const GLchar* vertSource = vertexShaderSource.c_str ();
	glShaderSource ( vertexShader, 1, &vertSource, 0 );
	glCompileShader ( vertexShader );

	GLint vertCompiled = 0;
	glGetShaderiv ( vertexShader, GL_COMPILE_STATUS, &vertCompiled );
	if ( vertCompiled == GL_FALSE ) {
		GLint maxLength = 0;
		glGetShaderiv ( vertexShader, GL_INFO_LOG_LENGTH, &maxLength );
		maxLength++;
		std::vector<GLchar> infoLog ( maxLength );
		glGetShaderInfoLog ( vertexShader, maxLength, &maxLength, &infoLog[0] );
		glDeleteShader ( vertexShader );
		std::cout << "There was an error compiling the vertex shader: ";
		for ( char i : infoLog ) {
			std::cout << i;
		}
		std::cout << std::endl;
		return;
	}

	GLuint fragmentShader = glCreateShader ( GL_FRAGMENT_SHADER );
	const GLchar* fragSource = fragmentShaderSource.c_str ();
	glShaderSource ( fragmentShader, 1, &fragSource, 0 );
	glCompileShader ( fragmentShader );

	GLint fragCompiled = 0;
	glGetShaderiv ( fragmentShader, GL_COMPILE_STATUS, &fragCompiled );
	if ( fragCompiled == GL_FALSE ) {
		GLint maxLength = 0;
		glGetShaderiv ( fragmentShader, GL_INFO_LOG_LENGTH, &maxLength );
		maxLength++;
		std::vector<GLchar> infoLog ( maxLength );
		glGetShaderInfoLog ( fragmentShader, maxLength, &maxLength, &infoLog[0] );
		glDeleteShader ( fragmentShader );
		std::cout << "There was an error compiling the fragment shader: ";
		for ( char i : infoLog ) {
			std::cout << i;
		}
		std::cout << std::endl;
		return;
	}

	program = glCreateProgram ();
	glAttachShader ( program, vertexShader );
	glAttachShader ( program, fragmentShader );
	glLinkProgram ( program );

	GLint isLinked = 0;
	glGetProgramiv ( program, GL_LINK_STATUS, (int*)&isLinked );
	if ( isLinked == GL_FALSE ) {
		GLint maxLength = 0;
		glGetProgramiv ( program, GL_INFO_LOG_LENGTH, &maxLength );
		maxLength++;
		std::vector<GLchar> infoLog ( maxLength );
		glGetProgramInfoLog ( program, maxLength, &maxLength, &infoLog[0] );
		glDeleteProgram ( program );
		glDeleteShader ( vertexShader );
		glDeleteShader ( fragmentShader );
		std::cout << "There was an error linking the program: ";
		for ( char i : infoLog ) {
			std::cout << i;
		}
		std::cout << std::endl;
		return;
	}

	glDetachShader ( program, vertexShader );
	glDetachShader ( program, fragmentShader );
	glDeleteShader ( vertexShader );
	glDeleteShader ( fragmentShader );
}

dap::Shader::~Shader () {
	glDeleteProgram ( program );
}

const GLuint* dap::Shader::GetProgram () {
	return &program;
}

void dap::Shader::Use () {
	glUseProgram ( program );
}
