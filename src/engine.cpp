#include "engine.h"

// public

void Engine::initialize() {

    glfwInit(); // ASSERT

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

    this->platform.width = ENGINE_WINDOW_WIDTH;
    this->platform.height = ENGINE_WINDOW_HEIGHT;
    this->platform.name = ENGINE_WINDOW_NAME;
    this->platform.window = glfwCreateWindow(this->platform.width, this->platform.height, this->platform.name.c_str(), NULL, NULL);
    // ASSERT

    glfwMakeContextCurrent(this->platform.window);
    // glfwSetInputMode(this->platform.window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

#ifndef __APPLE__
    glewExperimental = 1;
    glewInit(); // ASSERT
#endif

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_PROGRAM_POINT_SIZE);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

}

void Engine::update() {
    while (!glfwWindowShouldClose(this->platform.window)) {

        glfwSwapBuffers(this->platform.window);
        glfwPollEvents();
    }
}

void Engine::terminate() {
    glfwTerminate();
}

// private

int32_t main(void) {
    Engine engine;
    engine.initialize();
    engine.update();
    engine.terminate();
    return 0;
}