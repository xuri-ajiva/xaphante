#pragma once
#include <string>     
#include <GL/glew.h>

class ShaderHandler
{
public:
    ShaderHandler(const char* vertexShaderFilename, const char* fragmentShaderFilename);
    virtual ~ShaderHandler();

    void Bind();
    void Unbind();

private:

    GLuint Compile(std::string shaderSource, GLenum type);

    static std::string Parse(const char* filename);
    GLuint CreateShader(const char* vertexShaderFilename, const char* fragmentShaderFilename);

    GLuint SHADER_ID_;
};

