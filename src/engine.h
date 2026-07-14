#ifndef __ENGINE_H__
#define __ENGINE_H__

#ifdef __APPLE__
    #define GL_SILENCE_DEPRECATION
    #define GLFW_INCLUDE_GLCOREARB
    #include <GLFW/glfw3.h>
#else
    #include <GL/glew.h>
    #include <GLFW/glfw3.h>
#endif

#include "math.h" // temp
#include "shader.h"

#define ENGINE_WINDOW_WIDTH 1920 // px
#define ENGINE_WINDOW_HEIGHT 1080
#define ENGINE_WINDOW_NAME "ARM ENGINE (Build v0.0.1)"

#define ENGINE_GRID_CELL_WIDTH 10 // px

typedef struct {
    GLFWwindow *window;
    uint32_t width, height;
    std::string name;
} Platform_t;

class Engine {
public:
    void initialize();
    void update();
    void terminate();
private:
    Platform_t platform;
};

#endif // !__ENGINE_H__