#include "Shader.h"

#include <GLFW/glfw3.h>
#include <fstream>
#include <iostream>
#include <vector>

const std::string dap::Shader::VERTEX_START_LABEL = "<v>";
const std::string dap::Shader::FRAGMENT_START_LABEL = "<f>";
const std::string dap::Shader::VERTEX_END_LABEL = "</v>";
const std::string dap::Shader::FRAGMENT_END_LABEL = "</f>";

dap::Shader::Shader ( std::string name, std::string path ) {
	this->name = name;
	std::ifstream ifs;
	std::string completeShaderSource;

	ifs.open ( path, std::ifstream::in );
	while ( ifs.good () ) {
		char c = ifs.get ();
		if ( c >= 128 || c < 0 )
			continue;
		completeShaderSource += c;
	}
	ifs.close ();

	const unsigned int vertexStartIndex = completeShaderSource.find ( VERTEX_START_LABEL ) + VERTEX_START_LABEL.size ();
	const unsigned int fragmentStartIndex = completeShaderSource.find ( FRAGMENT_START_LABEL ) + FRAGMENT_START_LABEL.size ();
	const unsigned int vertexEndIndex = completeShaderSource.find ( VERTEX_END_LABEL ) - vertexStartIndex;
	const unsigned int fragmentEndIndex = completeShaderSource.find ( FRAGMENT_END_LABEL ) - fragmentStartIndex;
	const std::string vertexShaderSource = completeShaderSource.substr ( vertexStartIndex, vertexEndIndex );
	const std::string fragmentShaderSource = completeShaderSource.substr ( fragmentStartIndex, fragmentEndIndex );
	std::cout << "Compiling shader" << std::endl;

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

std::string dap::Shader::GetName () {
	return name;
}

void dap::Shader::Use () {
	glUseProgram ( program );
}
