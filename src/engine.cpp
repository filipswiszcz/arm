#include "engine.h"

// public

void Engine::initialize(void) {

    // glfw

    if (!ENGINE_ASSERT(glfwInit())) {
        // do smth with that bro
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_COCOA_RETINA_FRAMEBUFFER, GL_FALSE);
#endif
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

    this->platform.width = ENGINE_WINDOW_WIDTH;
    this->platform.height = ENGINE_WINDOW_HEIGHT;
    this->platform.name = ENGINE_WINDOW_NAME;
    this->platform.window = glfwCreateWindow(this->platform.width, this->platform.height, this->platform.name.c_str(), NULL, NULL);
    if (!ENGINE_ASSERT(this->platform.window)) {
        // do smth here as well
    }

    glfwMakeContextCurrent(this->platform.window);
    // glfwSetInputMode(this->platform.window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSwapInterval(1);

    // glew

#ifndef __APPLE__
    glewExperimental = 1;
    if (!ENGINE_ASSERT(glewInit())) {
        // we have a situation here
    }
#endif

    glViewport(0, 0, this->platform.width, this->platform.height);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_PROGRAM_POINT_SIZE);
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // do i need it here?

    // clock
    this->clock.lft = glfwGetTime();

}

void Engine::update(void) {
    while (!glfwWindowShouldClose(this->platform.window)) {

        // clock
        const double time = glfwGetTime();
        this->clock.dt = time - this->clock.lft;
        this->clock.lft = time;
        
        this->clock.framerate.timer += this->clock.dt;
        this->clock.framerate.counter++;
        if (this->clock.framerate.timer >= 1.0) {
            this->clock.framerate.timer -= 1.0;
            this->clock.framerate.counter = 0;
        }

        if (this->clock.dt > 0.25) {
            this->clock.dt = 0.25;
        }

        this->clock.phys.accum += this->clock.dt;
        
        while (this->clock.phys.accum >= ENGINE_CLOCK_SIMULATION_FIXED_TIMESTEP) {

            //..

            this->clock.phys.accum -= ENGINE_CLOCK_SIMULATION_FIXED_TIMESTEP;
        }

        this->clock.anim.accum += this->clock.dt;

        while (this->clock.anim.accum >= ENGINE_CLOCK_ANIMATION_FIXED_TIMESTEP) {

            //..

            this->clock.anim.accum -= ENGINE_CLOCK_ANIMATION_FIXED_TIMESTEP;
        }

        glfwSwapBuffers(this->platform.window);
        glfwPollEvents();
    }
}

void Engine::terminate(void) {
    glfwTerminate();
}

// private

int main(void) {
    Engine engine;
    engine.initialize();
    engine.update();
    engine.terminate();
    return 0;
}