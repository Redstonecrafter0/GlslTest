#include <iostream>

#include <glad/gl.h>
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

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cout << "usage: glsltest <shaderpath>" << std::endl;
        std::cout << "       shaderpath - path to shader dir" << std::endl;
        return -1;
    }

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

    if (gladLoadGL(glfwGetProcAddress) == 0) {
        glfwTerminate();
        return -1;
    }

    stbi_set_flip_vertically_on_load(true);
    stbi_flip_vertically_on_write(true);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    auto shaderConfig = ShaderConfig(argv[1]);

    glfwSetWindowSize(window, shaderConfig.width, shaderConfig.height);

    bool save = true;

    while (!glfwWindowShouldClose(window)) {
        glClearColor(0, 0, 0, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        shaderConfig.render(save);

        if (save) {
            save = false;
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    std::filesystem::remove("s"); // delete this file that gets created but i dont know how and where
    exit(0); // just ignore all occurring errors
}
