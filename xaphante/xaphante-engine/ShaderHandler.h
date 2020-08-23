#pragma once
#include <string>
#include <GL/glew.h>

class ShaderHandler {
public:
	ShaderHandler(std::string* vertexShaderFilename, std::string* fragmentShaderFilename);

	virtual ~ShaderHandler();

	void Bind();

	void Unbind();

	GLuint GetShaderId() const {
		return SHADER_ID_;
	}

private:

	GLuint Compile(std::string* shaderSource, GLenum type);

	static std::string Parse(std::string* filename);

	GLuint CreateShader(std::string* vertexShaderFilename, std::string* fragmentShaderFilename);

	GLuint SHADER_ID_;
};
