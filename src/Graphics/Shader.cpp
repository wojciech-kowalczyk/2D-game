#include "Shader.h"
#include <sstream>
#include <fstream>
#include "renderFunctions.h"
#include "config.h"

Shader::Shader(Type const type) {
	std::string vertexPath {shader::DIRECTORY_PATH};
	std::string fragmentPath {shader::DIRECTORY_PATH};
	
	if (type == Texture) {
		vertexPath += "/textureVertex.glsl";
		fragmentPath += "/textureFragment.glsl";
	} else if (type == Color) {
		vertexPath += "/colorVertex.glsl";
		fragmentPath += "/colorFragment.glsl";
	} else {
		vertexPath += "/textureAndColorVertex.glsl";
		fragmentPath += "/textureAndColorFragment.glsl";
	}
	
	auto fileToString = [](std::string const &filePath) {
		std::ifstream file {filePath};
		std::stringstream temp;
		temp << file.rdbuf();
		file.close();
		return std::move(temp.str());
	};
	
	using namespace renderFunctions::shader;
	
	struct {
		unsigned id;
		std::string code;
		char const *codePtr;
	} vertex {glCreateShader(GL_VERTEX_SHADER), std::move(fileToString(vertexPath))}, fragment {glCreateShader(GL_FRAGMENT_SHADER),
	                                                                                            std::move(fileToString(fragmentPath))};
	
	vertex.codePtr = vertex.code.c_str();
	glShaderSource(vertex.id, 1, &vertex.codePtr, nullptr);
	glCompileShader(vertex.id);
	
	fragment.codePtr = fragment.code.c_str();
	glShaderSource(fragment.id, 1, &fragment.codePtr, nullptr);
	glCompileShader(fragment.id);
	
	// Link the program
	id = glCreateProgram();
	glAttachShader(id, vertex.id);
	glAttachShader(id, fragment.id);
	glLinkProgram(id);
	
	glDetachShader(id, vertex.id);
	glDetachShader(id, fragment.id);
	
	glDeleteShader(vertex.id);
	glDeleteShader(fragment.id);
	
	activate();
	
	if (type == Texture || type == TextureColor)
		renderFunctions::uniform::glUniform1i(renderFunctions::uniform::glGetUniformLocation(id, "texture0"), 0);
}

Shader::~Shader() { renderFunctions::shader::glDeleteProgram(id); }

void Shader::activate() const { renderFunctions::shader::glUseProgram(id); }
