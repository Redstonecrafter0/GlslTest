#include "VertexArrayObject.h"

using namespace glo;

VertexArrayObject::VertexArrayObject(const json& config, const ShaderProgram& shaderProgram, const std::vector<GLfloat>& vertices, const std::vector<GLuint>& elements) {
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    size_t vertexSize = 0;
    for (auto& i : config["input_locations"].get<std::vector<json>>()) {
        vertexSize += i["size"].get<size_t>();
    }

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(vertices.size() * vertexSize * sizeof(GLfloat)), vertices.data(), GL_STATIC_DRAW);

    size_t pos = 0;
    for (auto& i : config["input_locations"].get<std::vector<json>>()) {
        GLint attributeLocation = glGetAttribLocation(shaderProgram.shaderProgram, i["name"].get<std::string>().c_str());
        glVertexAttribPointer(attributeLocation, i["size"].get<GLint>(), GL_FLOAT, GL_FALSE, static_cast<GLsizei>(sizeof(GLfloat) * vertexSize), reinterpret_cast<const void*>(pos * sizeof(GLfloat)));
        glEnableVertexAttribArray(attributeLocation);
        pos++;
    }

    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizeiptr>(elements.size() * sizeof(GLuint)), elements.data(), GL_STATIC_DRAW);

    elementsSize = static_cast<GLsizei>(elements.size() * vertexSize);
}

void VertexArrayObject::bind() const {
    glBindVertexArray(vao);
}

void VertexArrayObject::draw() const {
    bind();
    glDrawElements(GL_TRIANGLES, elementsSize, GL_UNSIGNED_INT, 0);
}
