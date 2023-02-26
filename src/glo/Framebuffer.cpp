#include "Framebuffer.h"

using namespace glo;

Framebuffer::Framebuffer(GLsizei width, GLsizei height, GLuint slot) {
    glGenFramebuffers(1, &fbo);
    texture = new Texture(width, height, slot, fbo);
}

void Framebuffer::bind() const {
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
}

Framebuffer::Framebuffer() : fbo(0) {}

Framebuffer::~Framebuffer() {
    if (fbo != 0) {
        delete texture;
        glDeleteFramebuffers(1, &fbo);
    }
}

Texture* Framebuffer::getTexture() const {
    return texture;
}
