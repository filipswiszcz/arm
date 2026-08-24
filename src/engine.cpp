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
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // do i need it here?

    // renderer
    this->renderer.initialize();
    this->renderer.read_shaders("res");

    this->renderer.init_preview_cube(); // preview
    this->renderer.init_preview_cone(); // preview
    // this->collider.pos = {0.0f, 2.0f, 0.0f}; // preview
    // this->collider.radius = 1.0f; // preview
    this->collider.min = {0.1f, 1.98f, -0.02f}; // preview
    this->collider.max = {1.0f, 2.02f, 0.02f}; // preview

    // camera
    this->camera.pos = {0.0f, 4.0f, 4.0f};
    this->camera.tpos = {0.0f, -0.5f, -1.0f};
    this->camera.hpos = {0.0f, 1.0f, 0.0f};
    this->camera.yaw = -90.0f;
    this->camera.pitch = 0.0f;
    this->camera.mx = 0.0;
    this->camera.my = 0.0;
    this->camera.speed = 4.0f;
    this->camera.sens = 0.1f;
    this->camera.lock = 0;

    // gizmo
    this->gizmo.type = GizmoType::TRANSLATE;
    this->gizmo.pos = {0.0f, 2.0f, 0.0f};
    this->gizmo.mx = 0.0;
    this->gizmo.my = 0.0;
    this->gizmo.colliders[0] = {{0.1f, 1.98f, -0.02f}, {1.0f, 2.02f, 0.02f}, 0};
    this->gizmo.colliders[1] = {{-0.02f, 2.1f, -0.02f}, {0.02f, 3.0f, 0.02f}, 0};
    this->gizmo.colliders[2] = {{-0.02f, 1.98f, 0.1f}, {0.02f, 2.02f, 1.0f}, 0};
    this->gizmo.colors[0] = {1.0f, 0.0f, 0.0f, 1.0f};
    this->gizmo.colors[1] = {0.0f, 1.0f, 0.0f, 1.0f};
    this->gizmo.colors[2] = {0.0f, 0.0f, 1.0f, 1.0f};
    this->gizmo.step = 0.1f;
    this->gizmo.visible = 1;
    this->gizmo.lock = 0;

    // engine
    this->mode = EngineMode::SELECTION;

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
        this->projection = perspective(radians(90.0f), ((float) this->platform.width / (float) this->platform.height), 0.01f, 100.0f);
        this->view = look_at(this->camera.pos, (this->camera.pos + this->camera.tpos), this->camera.hpos);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        this->renderer.push_cmd({1.0f, 1.0f, 1.0f, 1.0f}); // grid

        if (this->gizmo.visible) { // gizmo
            this->renderer.push_cmd(this->gizmo.pos + v3{0.1f, 0.0f, 0.0f}, this->gizmo.pos + v3{1.0f, 0.0f, 0.0f}, this->gizmo.colors[0], 2.0f);
            this->renderer.push_cmd(this->gizmo.pos + v3{0.0f, 0.1f, 0.0f}, this->gizmo.pos + v3{0.0f, 1.0f, 0.0f}, this->gizmo.colors[1], 2.0f);
            this->renderer.push_cmd(this->gizmo.pos + v3{0.0f, 0.0f, 0.1f}, this->gizmo.pos + v3{0.0f, 0.0f, 1.0f}, this->gizmo.colors[2], 2.0f);

            m4 models[3] = {m4{1.0f}, m4{1.0f}, m4{1.0f}};
            models[0] = scale(models[0], v3{0.04f, 0.1f, 0.04f});
            models[0] = rotate(models[0], 90.0f, v3{0.0f, 0.0f, 1.0f});
            models[0] = translate(models[0], this->gizmo.pos + v3{1.05f, 0.0f, 0.0f});
            this->renderer.push_cmd(1, 0, models[0], this->gizmo.colors[0]);
            models[1] = scale(models[1], v3{0.04f, 0.1f, 0.04f});
            models[1] = translate(models[1], (this->gizmo.pos + v3{0.0f, 1.05f, 0.0f}));
            this->renderer.push_cmd(1, 0, models[1], this->gizmo.colors[1]);
            models[2] = scale(models[2], v3{0.04f, 0.1f, 0.04f});
            models[2] = rotate(models[2], -90.0f, v3{1.0f, 0.0f, 0.0f});
            models[2] = translate(models[2], this->gizmo.pos + v3{0.0f, 0.0f, 1.05f});
            this->renderer.push_cmd(1, 0, models[2], this->gizmo.colors[2]);
        }

        Mat4_t model(1.0f);
        model = translate(model, {0.0f, 2.0f, 0.0f});
        this->renderer.push_cmd(0, 0, model, {0.0f, 0.0f, 0.0f, 1.0f}); // cube

        this->renderer.draw((this->projection * this->view), this->camera.pos);

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
        if (this->mode != EngineMode::ROAM) { // do it with events or smth (works for now)
            this->mode = EngineMode::ROAM;
            glfwSetInputMode(this->platform.window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            glfwSetCursorPos(this->platform.window, this->camera.mx, this->camera.my);
        }
    } else if (glfwGetMouseButton(this->platform.window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE) {
        if (this->mode != EngineMode::SELECTION) {
            this->mode = EngineMode::SELECTION;
            glfwSetInputMode(this->platform.window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }
    }
    if (this->mode == EngineMode::ROAM) {
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

void Engine::handle_mouse(void) { // handle more things than only camera hrere lol
    f64 mx, my;
    glfwGetCursorPos(this->platform.window, &mx, &my);
    
    if (this->mode == EngineMode::SELECTION) {
        f32 x = (2.0f * mx) / this->platform.width - 1.0f;
        f32 y = 1.0f - (2.0f * my) / this->platform.height;

        v4 clip_pos = {x, y, -1.0f, 1.0f};
        v4 ray_view = inverse(this->projection) * clip_pos;
        ray_view.z = -1.0f;
        ray_view.w = 0.0f;

        v4 ray = (inverse(this->view) * ray_view);
        v3 direction = normalize({ray.x, ray.y, ray.z});

        // if (check_sphere_intersection(this->camera.pos, direction, this->collider.pos, this->collider.radius)) {
        //     // std::cout << "hit" << std::endl;
        // } else {}

        if (glfwGetMouseButton(this->platform.window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE) {
            this->gizmo.lock = 0;
            glfwSetInputMode(this->platform.window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }

        if (!this->gizmo.lock) {
            for (u32 i = 0; i < 3; i++) {
                if (check_aabb_collision(this->camera.pos, direction, this->gizmo.colliders[i])) {
                    if ((glfwGetMouseButton(this->platform.window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)) {
                        this->gizmo.axis = i;
                        this->gizmo.lock = 1;
                        glfwSetInputMode(this->platform.window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
                        break;
                    }
                    this->gizmo.colors[i].w = 0.8f;
                } else {
                    this->gizmo.colors[i].w = 1.0f;
                }
            }
        }

        if (this->gizmo.lock) {
            f32 xdelta = ((mx - this->gizmo.mx) * 0.01f);
            f32 ydelta = ((this->gizmo.my - my) * 0.01f);

            f32 xstep = 0.0f;
            f32 ystep = 0.0f;

            if (std::abs(xdelta) >= this->gizmo.step) {
                xstep = std::trunc(xdelta / this->gizmo.step) * this->gizmo.step;
                this->gizmo.mx += (xstep / 0.01f);
            }
            if (std::abs(ydelta) >= this->gizmo.step) {
                ystep = std::trunc(ydelta / this->gizmo.step) * this->gizmo.step;
                this->gizmo.my -= (ystep / 0.01f);
            }

            if (this->gizmo.axis == 0 && xstep != 0.0f) {
                this->gizmo.pos.x += xstep;
                for (u32 i = 0; i < 3; i++) {
                    this->gizmo.colliders[i].min.x += xstep;
                    this->gizmo.colliders[i].max.x += xstep;
                }
            } else if (this->gizmo.axis == 1 && ystep != 0.0f) {
                this->gizmo.pos.y += ystep;
                for (u32 i = 0; i < 3; i++) {
                    this->gizmo.colliders[i].min.y += ystep;
                    this->gizmo.colliders[i].max.y += ystep;
                }
            } else if (this->gizmo.axis == 2 && ystep != 0.0f) {
                this->gizmo.pos.z -= ystep;
                for (u32 i = 0; i < 3; i++) {
                    this->gizmo.colliders[i].min.z -= ystep;
                    this->gizmo.colliders[i].max.z -= ystep;
                }
            }
        }

        if (!this->gizmo.lock) {
            this->gizmo.mx = mx;
            this->gizmo.my = my;
        }
    }

    if (this->mode == EngineMode::ROAM) {
        f32 offset_x = ((mx - this->camera.mx) * this->camera.sens);
        f32 offset_y = ((this->camera.my - my) * this->camera.sens);

        this->camera.mx = mx;
        this->camera.my = my;

        this->camera.yaw = (this->camera.yaw + offset_x);
        this->camera.pitch = (this->camera.pitch + offset_y);

        if (this->camera.pitch > 89.0f) this->camera.pitch = 89.0f;
        if (this->camera.pitch < -89.0f) this->camera.pitch = -89.0f;

        v3 target = {
            (cos(radians(this->camera.yaw)) * cos(radians(this->camera.pitch))),
            sin(radians(this->camera.pitch)),
            (sin(radians(this->camera.yaw)) * cos(radians(this->camera.pitch)))
        };

        this->camera.tpos = normalize(target);
    }
}

void Engine::update_gizmo(void) {}

// other

i32 main(void) {
    Engine engine;
    engine.initialize();
    engine.update();
    engine.terminate();
    return 0;
}