#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "glo/ShaderConfig.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

using namespace glo;

GLsizei width = 1280;
GLsizei height = 720;

void windowResizeCallback(GLFWwindow*, int w, int h) {
    width = w;
    height = h;
    glViewport(0, 0, width, height);
}

void errlog(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam) {
    std::cerr << std::string(message, length) << std::endl;
}

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cout << "usage: glsltest <shaderpath>" << std::endl;
        std::cout << "       shaderpath - path to shader dir or shader collection file" << std::endl;
        return -1;
    }

    if (!glfwInit()) {
        return -1;
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

//    std::ifstream configFile(argv[1]);
//    json config = json::parse(configFile);
//    configFile.close();
//
//    if (config.contains("width")) {
//        width = config["width"].get<GLsizei>();
//    }
//    if (config.contains("height")) {
//        height = config["height"].get<GLsizei>();
//    }

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

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(errlog, nullptr);

    auto shaderConfig = ShaderConfig(argv[1]);

    bool save = true;

    while (!glfwWindowShouldClose(window)) {
        glClearColor(0, 0, 0, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        shaderConfig.render(save);

        if (save) {
            save = false;
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
