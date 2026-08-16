#ifndef ARM_ENGINE_H
#define ARM_ENGINE_H

#ifdef __APPLE__
    #define GL_SILENCE_DEPRECATION
    #define GLFW_INCLUDE_GLCOREARB
    #include <GLFW/glfw3.h>
#else
    #include <GL/glew.h>
    #include <GLFW/glfw3.h>
#endif

#include "common.h"
#include "math.h" // temp
#include "renderer.h"
// #include "shader.h"
#include "simulation.h"

#define ENGINE_WINDOW_WIDTH 1920 // px
#define ENGINE_WINDOW_HEIGHT 1080
#define ENGINE_WINDOW_NAME "ARM ENGINE (Build v0.0.1)"

#define ENGINE_CLOCK_SIMULATION_FIXED_TIMESTEP (1.0 / 60.0) // 60 fps
#define ENGINE_CLOCK_ANIMATION_FIXED_TIMESTEP (1.0 / 8.0) // 8 fps

#define ENGINE_GRID_CELL_WIDTH 10 // px

typedef struct {
    GLFWwindow *window;
    u32 width, height;
    std::string name; // dont use string u mf
    i32 keys[512]; // weirdo

} Platform_t;

typedef struct {
    struct {f64 accum;} phys;
    struct {f64 accum;} anim;
    
    struct {
        f64 timer;
        u32 counter;
    } framerate;

    f64 lft, dt; // last frame time, delta time
} Clock_t;

typedef struct {
    v3 pos, tpos, hpos;
    f32 yaw, pitch;
    f64 mx, my;
    f32 speed, sens;
    u8 lock;
} Camera_t;

enum class EngineMode : u8 {
    SELECTION,
    ROAM
};

class Engine {
public:
    void initialize(void);
    void update(void);
    void terminate(void);
private:
    Platform_t platform;
    Clock_t clock;
    
    Renderer::Renderer renderer;

    Camera_t camera;

    EngineMode mode;
    
    // Simulation simulation;
    // grid polygons init

    static void callback_key(GLFWwindow *window, i32 key, i32 scancode, i32 action, i32 mods);
    // static void callback_mouse(GLFWwindow *window, i32 button, i32 action, i32 mods);
    void register_key(i32 key, i32 action);
    void handle_keyboard(void);
    void handle_mouse(void);
};

#endif // !ARM_ENGINE_H