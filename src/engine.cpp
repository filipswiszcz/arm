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
    glfwSetWindowUserPointer(this->platform.window, this);
    glfwSetKeyCallback(this->platform.window, callback_key);
    glfwSetInputMode(this->platform.window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
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
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // do i need it here?

    // renderer
    this->renderer = Renderer::Renderer();
    this->renderer.initialize();
    this->renderer.read_shaders("res");

    // camera
    this->camera.pos = {0.0f, 4.0f, 8.0f};
    this->camera.tpos = {0.0f, -0.5f, -1.0f};
    this->camera.hpos = {0.0f, 1.0f, 0.0f};
    this->camera.yaw = -90.0f;
    this->camera.pitch = 0.0f;
    this->camera.speed = 4.0f;
    this->camera.sens = 0.1f;
    this->camera.lock = 0;

    // preview
    this->renderer.init_preview_cube();

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

            this->handle_keyboard();
            this->handle_mouse();

            //..

            this->clock.phys.accum -= ENGINE_CLOCK_SIMULATION_FIXED_TIMESTEP;
        }

        this->clock.anim.accum += this->clock.dt;

        while (this->clock.anim.accum >= ENGINE_CLOCK_ANIMATION_FIXED_TIMESTEP) {

            //..

            this->clock.anim.accum -= ENGINE_CLOCK_ANIMATION_FIXED_TIMESTEP;
        }

        // renderer
        Mat4_t projection = perspective(radians(90.0f), ((float) this->platform.width / (float) this->platform.height), 0.1f, 1000.0f);
        Mat4_t view = look_at(this->camera.pos, (this->camera.pos + this->camera.tpos), this->camera.hpos);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        Mat4_t model(1.0f);

        // this->renderer.push_cmd(Vec4(1.0f, 1.0f, 1.0f, 1.0f));
        this->renderer.push_cmd({1.0f, 1.0f, 1.0f, 1.0f});
        this->renderer.push_cmd(0, 0, model, Vec4{0.0f, 0.0f, 0.0f, 1.0f});
        this->renderer.draw((projection * view), this->camera.pos);

        glfwSwapBuffers(this->platform.window);
        glfwPollEvents();
    }
}

void Engine::terminate(void) {
    glfwTerminate();
}

// private

void Engine::callback_key(GLFWwindow *window, i32 key, i32 scancode, i32 action, i32 mods) {
    Engine *engine = static_cast<Engine*>(glfwGetWindowUserPointer(window));
    if (engine != nullptr) engine->register_key(key, action);
}

void Engine::register_key(i32 key, i32 action) {
    if (key < 0 && key > 511) return;
    if (action == GLFW_PRESS) this->platform.keys[key] = 1;
    else if (action == GLFW_RELEASE) this->platform.keys[key] = 0;
}

void Engine::handle_keyboard(void) {
    if (glfwGetMouseButton(this->platform.window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
        if (this->platform.keys[GLFW_KEY_W] == GLFW_PRESS) {
            this->camera.pos = (this->camera.pos + (this->camera.tpos * this->camera.speed * this->clock.dt));
        }
        if (this->platform.keys[GLFW_KEY_S] == GLFW_PRESS) {
            this->camera.pos = (this->camera.pos - (this->camera.tpos * this->camera.speed * this->clock.dt));
        }
        if (this->platform.keys[GLFW_KEY_A] == GLFW_PRESS) {
            this->camera.pos = (this->camera.pos - (normalize(cross(this->camera.tpos, this->camera.hpos)) * (this->camera.speed * this->clock.dt)));
        }
        if (this->platform.keys[GLFW_KEY_D] == GLFW_PRESS) {
            this->camera.pos = (this->camera.pos + (normalize(cross(this->camera.tpos, this->camera.hpos)) * (this->camera.speed * this->clock.dt)));
        }
    }
    if (this->platform.keys[GLFW_KEY_ESCAPE] == GLFW_PRESS) {
        glfwSetWindowShouldClose(this->platform.window, 1);
    }
}

void Engine::handle_mouse(void) {
    double mx, my;
    glfwGetCursorPos(this->platform.window, &mx, &my);

    float offsetx = ((mx - this->camera.mx) * this->camera.sens);
    float offsety = ((this->camera.my - my) * this->camera.sens);

    this->camera.mx = mx;
    this->camera.my = my;

    this->camera.yaw = (this->camera.yaw + offsetx);
    this->camera.pitch = (this->camera.pitch + offsety);

    if (this->camera.pitch > 89.0f) this->camera.pitch = 89.0f;
    if (this->camera.pitch < -89.0f) this->camera.pitch = -89.0f;

    v3 target = {
        (cos(radians(this->camera.yaw)) * cos(radians(this->camera.pitch))),
        sin(radians(this->camera.pitch)),
        (sin(radians(this->camera.yaw)) * cos(radians(this->camera.pitch)))
    };

    this->camera.tpos = normalize(target);
}

// other

int main(void) {
    Engine engine;
    engine.initialize();
    engine.update();
    engine.terminate();
    return 0;
}