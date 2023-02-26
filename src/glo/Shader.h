#pragma once

#include <GL/glew.h>
#include <fstream>

namespace glo {

class Shader {
public:
    GLuint shader;
    GLenum type;

    Shader(std::ifstream srcFile, GLenum shaderType);
};

}
