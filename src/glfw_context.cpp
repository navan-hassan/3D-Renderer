#include <glfw_context.hpp>
#include <logging_utilities.hpp>

GLFWContext::GLFWContext() {
	glfwInit();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	logging::DEBUG("SUCCESSFULLY CREATED GLFW CONTEXT");
}

GLFWContext::~GLFWContext() {
	glfwTerminate();
	logging::DEBUG("SUCCESSFULLY DESTROYED GLFW CONTEXT");
}

std::vector<const char*> GLFWContext::getRequiredInstanceExtensions() const {
	uint32_t requiredExtensionCount = 0;
	const char** pRequiredExtensionNames;
	pRequiredExtensionNames = glfwGetRequiredInstanceExtensions(&requiredExtensionCount);

	std::vector<const char*> requiredExtensions(pRequiredExtensionNames, pRequiredExtensionNames + requiredExtensionCount);
	return requiredExtensions;
}

std::shared_ptr<GLFWContext> GLFWContext::createSharedInstance() {
	return std::make_shared<GLFWContext>();
}