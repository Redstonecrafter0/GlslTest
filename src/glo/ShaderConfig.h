#pragma once

#include <filesystem>
#include "Framebuffer.h"
#include "VertexArrayObject.h"

namespace glo {

class ShaderConfig {
public:
    GLsizei width;
    GLsizei height;

    ShaderConfig(const std::filesystem::path& shaderDir, GLint slot = -1);
    ~ShaderConfig();

    void render(bool save, GLint time) const;

    Framebuffer* getFramebuffer() const;

private:
    std::filesystem::path shaderDir;
    Framebuffer* framebuffer;
    VertexArrayObject* vao;
    ShaderProgram* shaderProgram;
    std::vector<ShaderConfig*> dependencies;
};

}
