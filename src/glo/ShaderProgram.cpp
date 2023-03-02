#include "ShaderProgram.h"
#include "Texture.h"

#include <GLFW/glfw3.h>
#include <iostream>
#include <map>
#include "ShaderConfig.h"

using namespace glo;

ShaderProgram::ShaderProgram(const Shader& vertexShader, const Shader& fragmentShader) : shaderProgram(glCreateProgram()) {
    glAttachShader(shaderProgram, vertexShader.shader);
    glAttachShader(shaderProgram, fragmentShader.shader);
    glLinkProgram(shaderProgram);

    GLint linked;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &linked);
    if (!linked) {
        GLint len;
        glGetProgramiv(shaderProgram, GL_INFO_LOG_LENGTH, &len);
        GLint errLen;
        GLchar err[len];
        glGetProgramInfoLog(shaderProgram, len, &errLen, err);
        auto errLog = std::string(err, errLen);
        std::cerr << "Failed to link shader program!" << std::endl;
        std::cerr << errLog << std::endl;
        glfwTerminate();
        exit(-1);
    }
}

ShaderProgram::~ShaderProgram() {
    for (auto& i : textures) {
        delete i;
    }
}

void ShaderProgram::use() const {
    glUseProgram(shaderProgram);
}

void ShaderProgram::loadUniforms(const json& config, const std::filesystem::path& imagePath, std::vector<ShaderConfig*>& dependencies) {
    use();
    auto uniformLocations = std::map<std::string, GLint>();
    GLint uniformCount;
    glGetProgramiv(shaderProgram, GL_ACTIVE_UNIFORMS, &uniformCount);
    GLint maxStrLen;
    glGetProgramiv(shaderProgram, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxStrLen);

    GLint currentTextureSlot = 0;

    for (GLint i = 0; i < uniformCount; ++i) {
        GLsizei strLen;
        GLint uniformSize;
        GLenum uniformType;
        GLchar rawName[maxStrLen];
        glGetActiveUniform(shaderProgram, i, maxStrLen, &strLen, &uniformSize, &uniformType, rawName);
        auto name = std::string(rawName, strLen);
        if (name.compare(name.length() - 3, 3, "[0]") == 0) {
            name = name.substr(0, name.length() - 3);
        }
        auto value = config["uniform"][name];
        size_t valSize;
        switch (uniformType) {
            case GL_FLOAT_VEC2:
                valSize = 2;
                break;
            case GL_FLOAT_VEC3:
                valSize = 3;
                break;
            case GL_FLOAT_VEC4:
                valSize = 4;
                break;
            case GL_FLOAT_MAT2:
                valSize = 2;
                break;
            case GL_FLOAT_MAT3:
                valSize = 3;
                break;
            case GL_FLOAT_MAT4:
                valSize = 4;
                break;
            default:
                valSize = 1;
                break;
        }
        switch (uniformType) {
            case GL_FLOAT: {
                auto vec = value.get<std::vector<GLfloat>>();
                glUniform1fv(i, static_cast<GLsizei>(vec.size()), vec.data());
                break;
            }
            case GL_INT: {
                auto vec = value.get<std::vector<GLint>>();
                glUniform1iv(i, static_cast<GLsizei>(vec.size()), vec.data());
                break;
            }
            case GL_BOOL: {
                auto vec = value.get<std::vector<bool>>();
                GLint ints[vec.size()];
                for (int j = 0; j < vec.size(); ++j) {
                    ints[j] = vec[j] ? 1 : 0;
                }
                glUniform1iv(i, static_cast<GLsizei>(vec.size()), ints);
                break;
            }
            case GL_FLOAT_VEC2:
            case GL_FLOAT_VEC3:
            case GL_FLOAT_VEC4: {
                auto vec = value.get<std::vector<GLfloat>>();
                glUniform2fv(i, valSize, vec.data());
                break;
            }
            case GL_FLOAT_MAT2:
            case GL_FLOAT_MAT3:
            case GL_FLOAT_MAT4: {
                auto vec = value.get<std::vector<GLfloat>>();
                glUniformMatrix2fv(i, static_cast<GLsizei>(vec.size() / valSize), GL_FALSE, vec.data());
                break;
            }
            case GL_SAMPLER_2D: {
                auto vec = value.get<std::vector<std::string>>();
                std::vector<GLint> slots;
                for (const auto& j : vec) {
                    if (j.compare(j.length() - 4, 4, ".png") == 0) {
                        textures.emplace_back(new Texture(currentTextureSlot, (imagePath / j).string()));
                    } else {
                        auto shaderConfig = new ShaderConfig(imagePath / j, currentTextureSlot);
                        use();
                        dependencies.emplace_back(shaderConfig);
                        textures.emplace_back(shaderConfig->getFramebuffer()->getTexture());
                    }
                    slots.push_back(currentTextureSlot);
                    currentTextureSlot++;
                }
                glUniform1iv(i, static_cast<GLsizei>(slots.size()), slots.data());
                break;
            }
        }
    }
}

void ShaderProgram::updateUniformTextures() {
    for (auto& i : textures) {
        i->bind();
    }
}
