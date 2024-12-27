#ifndef RENDER_HPP
#define RENDER_HPP

#include <GLFW/glfw3.h>
#include <render_window.hpp>

// Class that handles logic for rendering on a GLFW window 
class Renderer {
public :
    void run();

private:
    Window window;
};

#endif