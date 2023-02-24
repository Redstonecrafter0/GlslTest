#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <nlohmann/json.hpp>
#include <fstream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

using json = nlohmann::json;

GLsizei width = 1280;
GLsizei height = 720;

void windowResizeCallback(GLFWwindow* window, int w, int h) {
    width = w;
    height = h;
}

GLuint compileShader(const json& config, GLenum shaderType, const std::string& type, const std::filesystem::path& shaderPath) {
    GLuint shader = glCreateShader(shaderType);
    std::ifstream srcFile(shaderPath / config[type].get<std::string>());
    std::string src;
    char c;
    while (srcFile.good()) {
        srcFile.get(c);
        src += c;
    }
    const char* srcC = src.c_str();
    const auto srcLen = static_cast<GLint>(src.length());
    glShaderSource(shader, 1, &srcC, &srcLen);
    srcFile.close();
    glCompileShader(shader);
    GLint compiled;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
    if (!compiled) {
        GLint len;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);
        GLint errLen;
        GLchar err[len];
        glGetShaderInfoLog(shader, len, &errLen, err);
        auto errLog = std::string(err, errLen);
        std::cerr << "Failed to compile vertex shader!" << std::endl;
        std::cerr << errLog << std::endl;
        glfwTerminate();
        exit(-1);
    }
    return shader;
}

int main(int argc, char** argv) {
    if (!(argc == 2 || argc == 4)) {
        std::cout << "usage: glsltest config.json <width> <height>" << std::endl;
    }

    if (argc == 4) {
        width = std::stoi(argv[2]);
        height = std::stoi(argv[3]);
    }

    std::ifstream configFile(argv[1]);
    json config = json::parse(configFile);
    configFile.close();

    if (!glfwInit()) {
        return -1;
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    GLFWwindow* window = glfwCreateWindow(width, height, "GlslTest", nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    glfwSetWindowSizeCallback(window, windowResizeCallback);
    glfwShowWindow(window);

    if (glewInit() != GLEW_OK) {
        glfwTerminate();
        return -1;
    }

    stbi_set_flip_vertically_on_load(true);
    stbi_flip_vertically_on_write(true);

    const auto vertices = config["input_data"]["vertices"].get<std::vector<GLfloat>>();

    const auto elements = config["input_data"]["elements"].get<std::vector<GLuint>>();

    std::filesystem::path configPath = argv[1];
    std::filesystem::path shaderPath = configPath.parent_path();
    GLuint vertexShader = compileShader(config, GL_VERTEX_SHADER, "vertex", shaderPath);
    GLuint fragmentShader = compileShader(config, GL_FRAGMENT_SHADER, "fragment", shaderPath);
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
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

    glUseProgram(shaderProgram);

    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    size_t vertexSize = 0;
    for (auto& i : config["input_locations"].get<std::vector<json>>()) {
        vertexSize += i["size"].get<size_t>();
    }

    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(vertices.size() * vertexSize * sizeof(GLfloat)), vertices.data(), GL_STATIC_DRAW);

    size_t pos = 0;
    for (auto& i : config["input_locations"].get<std::vector<json>>()) {
        GLint attributeLocation = glGetAttribLocation(shaderProgram, i["name"].get<std::string>().c_str());
        glVertexAttribPointer(attributeLocation, i["size"].get<GLint>(), GL_FLOAT, GL_FALSE, static_cast<GLsizei>(sizeof(GLfloat) * vertexSize), reinterpret_cast<const void*>(pos * sizeof(GLfloat)));
        glEnableVertexAttribArray(attributeLocation);
        pos++;
    }

    GLuint ebo;
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizeiptr>(elements.size() * sizeof(GLuint)), elements.data(), GL_STATIC_DRAW);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    auto uniformLocations = std::map<std::string, GLint>();
    GLint uniformCount;
    glGetProgramiv(shaderProgram, GL_ACTIVE_UNIFORMS, &uniformCount);
    GLint maxStrLen;
    glGetProgramiv(shaderProgram, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxStrLen);

    GLint currentTextureSlot = 1;

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
                    GLuint texture;
                    glGenTextures(1, &texture);
                    glActiveTexture(GL_TEXTURE0 + currentTextureSlot);
                    glBindTexture(GL_TEXTURE_2D, texture);
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
                    int w;
                    int h;
                    int channels;
                    std::string path = shaderPath / j;
                    unsigned char* data = stbi_load(path.c_str(), &w, &h, &channels, 0);
                    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
                    glGenerateMipmap(GL_TEXTURE_2D);
                    stbi_image_free(data);
                    slots.push_back(currentTextureSlot);
                    currentTextureSlot++;
                }
                glUniform1iv(i, static_cast<GLsizei>(slots.size()), slots.data());
                break;
            }
        }
    }

    bool saved = false;

    while (!glfwWindowShouldClose(window)) {
        glViewport(0, 0, width, height);
        glClearColor(0, 0, 0, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glDrawElements(GL_TRIANGLES, elements.size() * vertexSize, GL_UNSIGNED_INT, 0);

        if (!saved) {
            unsigned char buffer[width * height * 4];
            glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
            std::string outputPng = shaderPath / "output.png";
            stbi_write_png(outputPng.c_str(), width, height, 4, buffer, width * 4);
            saved = true;
        }

        GLenum err = glGetError();
        if (err != GL_NO_ERROR) {
            std::cerr << "error: " << gluErrorString(err) << std::endl;
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    return 0;
}
