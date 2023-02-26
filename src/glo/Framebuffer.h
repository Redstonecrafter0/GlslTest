#pragma once

#include <GL/glew.h>
#include "Texture.h"

namespace glo {

class Framebuffer {
public:
    GLuint fbo;

    Framebuffer(GLsizei width, GLsizei height, GLuint slot);
    Framebuffer();
    ~Framebuffer();

    void bind() const;

    Texture* getTexture() const;
private:
    Texture* texture;
};

}
