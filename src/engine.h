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
#include "shader.h"

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

class Engine {
public:
    void initialize(void);
    void update(void);
    void terminate(void);
private:
    Platform_t platform;
    Clock_t clock;
};

#endif // !ARM_ENGINE_H