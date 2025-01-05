#include <renderer.hpp>
#include <iostream>
#include <debug_utilities.hpp>

void Renderer::run() {

	debug_write("Starting Renderer...");

	window.init(800, 600);
	vulkanContext.init();
	vulkanContext.createSurface(window.window);
	vulkanContext.selectPhysicalDevice();
	window.mainLoop();
	terminate();
}
 
void Renderer::terminate() {

	vulkanContext.destroy();
	debug_write("Terminated Renderer");
}