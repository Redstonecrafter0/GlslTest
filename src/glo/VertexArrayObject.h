#pragma once

#include <nlohmann/json.hpp>
#include <vector>
#include "ShaderProgram.h"

namespace glo {

using json = nlohmann::json;

class VertexArrayObject {
public:
    GLuint vao;
    GLuint vbo;
    GLuint ebo;

    VertexArrayObject(const json& config, const ShaderProgram& shaderProgram, const std::vector<GLfloat>& vertices, const std::vector<GLuint>& elements);

    void bind() const;

    void draw() const;
private:
    GLsizei elementsSize;
};

}
