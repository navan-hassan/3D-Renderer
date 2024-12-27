#ifndef RENDER_H
#define RENDER_H

#include <GLFW/glfw3.h>

// Class that handles logic for rendering on a GLFW window 
class Renderer {
public :
    void run();

private:
    GLFWwindow* window = nullptr;
};

#endif