#pragma once

#include <glad/gl.h>
#include <nlohmann/json.hpp>
#include "Shader.h"
#include "Texture.h"

namespace glo {

using json = nlohmann::json;

class ShaderConfig;

class ShaderProgram {
public:
    GLuint shaderProgram;

    ShaderProgram(const Shader& vertexShader, const Shader& fragmentShader);
    ~ShaderProgram();

    void use() const;

    void loadUniforms(const json& config, const std::filesystem::path& imagePath, std::vector<ShaderConfig*>& dependencies);

private:
    std::vector<Texture*> textures;
};

}
