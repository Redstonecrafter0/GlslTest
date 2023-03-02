#include "Shader.h"

#include <GLFW/glfw3.h>
#include <iostream>

using namespace glo;

Shader::Shader(std::ifstream& srcFile, GLenum shaderType) : shader(glCreateShader(shaderType)), type(shaderType) {
    std::string src;
    char c;
    while (srcFile.good()) {
        srcFile.get(c);
        src += c;
    }
    const char* srcC = src.c_str();
    const auto srcLen = static_cast<GLint>(src.length());
    glShaderSource(shader, 1, &srcC, &srcLen);
    srcFile.close();
    glCompileShader(shader);
    GLint compiled;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
    if (!compiled) {
        GLint len;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);
        GLint errLen;
        GLchar err[len];
        glGetShaderInfoLog(shader, len, &errLen, err);
        auto errLog = std::string(err, errLen);
        std::cerr << "Failed to compile " << ((shaderType == GL_VERTEX_SHADER) ? "vertex" : "fragment") << " shader!" << std::endl;
        std::cerr << errLog << std::endl;
        glfwTerminate();
        exit(-1);
    }
}
