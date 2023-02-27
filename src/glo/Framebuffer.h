#pragma once

#include <glad/gl.h>
#include "Texture.h"

namespace glo {

class Framebuffer {
public:
    GLuint fbo;

    Framebuffer(GLsizei width, GLsizei height, GLuint slot);
    Framebuffer(GLsizei width, GLsizei height);
    ~Framebuffer();

    void bind();
    void save(const std::filesystem::path& outputPath);

    Texture* getTexture() const;
private:
    Texture* texture;
    GLsizei width;
    GLsizei height;
};

}
