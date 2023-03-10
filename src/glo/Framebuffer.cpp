#include "Framebuffer.h"
#include <iostream>

#include "stb_image_write.h"

using namespace glo;

Framebuffer::Framebuffer(GLsizei width, GLsizei height, GLuint slot) : width(width), height(height) {
    glGenFramebuffers(1, &fbo);
    texture = new Texture(width, height, slot, fbo);
}

Framebuffer::Framebuffer(GLsizei width, GLsizei height) : fbo(0), width(width), height(height) {}

Framebuffer::~Framebuffer() {
    if (fbo != 0) {
        delete texture;
        glDeleteFramebuffers(1, &fbo);
    }
}

void Framebuffer::bind() {
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glViewport(0, 0, width, height);
}

Texture* Framebuffer::getTexture() const {
    return texture;
}

void Framebuffer::save(const std::filesystem::path& outputPath) {
    bind();
    auto buffer = new unsigned char[width * height * 4];
    glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
    stbi_write_png(reinterpret_cast<const char*>(outputPath.string().c_str()), width, height, 4, buffer, width * 4);
    delete[] buffer;
}
