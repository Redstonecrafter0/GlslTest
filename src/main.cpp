#include <iostream>
#include <chrono>

#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include "glo/ShaderConfig.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

using namespace glo;
using namespace std::chrono;

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
        std::cerr << "GLFW failed initializing" << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    GLFWwindow* window = glfwCreateWindow(width, height, "GlslTest", nullptr, nullptr);
    if (!window) {
        std::cerr << "GLFW failed creating window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(0);
    glfwSetWindowSizeCallback(window, windowResizeCallback);
    glfwShowWindow(window);

    if (gladLoadGL(glfwGetProcAddress) == 0) {
        std::cerr << "GLAD failed loading" << std::endl;
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
    bool benchmarkDone = false;
    auto start = steady_clock::now();

    uint32_t frames = 0;

    while (!glfwWindowShouldClose(window)) {
        glClearColor(0, 0, 0, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        shaderConfig.render(save, static_cast<GLint>(duration_cast<milliseconds>(steady_clock::now() - start).count()));

        if (save) {
            save = false;
            start = steady_clock::now();
        } else if (!benchmarkDone && steady_clock::now() - start > seconds(10)) {
            benchmarkDone = true;
            std::cout << "Benchmarked: " << frames / 10.0 << " FPS" << std::endl;
            glfwSwapInterval(1);
        } else if (!benchmarkDone) {
            frames++;
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    exit(0); // just ignore all occurring errors
}
