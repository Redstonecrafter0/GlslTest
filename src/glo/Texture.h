#pragma once

#include <GL/glew.h>
#include <filesystem>
#include <string>

namespace glo {

class Texture {
public:
    GLuint texture;

    Texture(GLuint slot, const std::string& path);
    Texture(GLsizei width, GLsizei height, GLuint slot, GLuint framebuffer);
    ~Texture();

    void bind() const;
    void save(const std::filesystem::path& outputPath) const;

private:
    GLuint slot;
    GLsizei width;
    GLsizei height;

    void init();
};

}
