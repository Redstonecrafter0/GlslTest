#include "ShaderConfig.h"

#include "Shader.h"
#include "ShaderProgram.h"

#include <GL/glew.h>
#include <nlohmann/json.hpp>

#include "stb_image_write.h"

using namespace glo;

ShaderConfig::ShaderConfig(const std::filesystem::path& shaderDir, GLint slot) : shaderDir(shaderDir) {
    std::ifstream configFile(shaderDir / "shader.json");
    json config = json::parse(configFile);
    configFile.close();

    auto vertices = config["input_data"]["vertices"].get<std::vector<GLfloat>>();
    auto elements = config["input_data"]["elements"].get<std::vector<GLuint>>();

    auto vertexShader = Shader(shaderDir / config["vertex"].get<std::string>(), GL_VERTEX_SHADER);
    auto fragmentShader = Shader(shaderDir / config["fragment"].get<std::string>(), GL_FRAGMENT_SHADER);
    shaderProgram = new ShaderProgram(vertexShader, fragmentShader);
    shaderProgram->use();

    vao = new VertexArrayObject(config, *shaderProgram, vertices, elements);

    shaderProgram->loadUniforms(config, shaderDir, dependencies);

    width = config["width"].get<GLsizei>();
    height = config["height"].get<GLsizei>();

    if (slot == -1) {
        framebuffer = new Framebuffer(width, height);
    } else {
        framebuffer = new Framebuffer(width, height, slot);
    }
}

ShaderConfig::~ShaderConfig() {
    for (auto& i : dependencies) {
        delete i;
    }
    delete shaderProgram;
    delete vao;
    delete framebuffer;
}

void ShaderConfig::render(bool save) const {
    for (auto& i : dependencies) {
        i->render(save);
    }
    framebuffer->bind();
    shaderProgram->use();
    vao->draw();
    framebuffer->save(shaderDir / "output.png");
}

Framebuffer* ShaderConfig::getFramebuffer() const {
    return framebuffer;
}
