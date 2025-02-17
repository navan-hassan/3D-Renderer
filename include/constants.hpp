#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

#include <string>

namespace constants {
	constexpr const char* WINDOW_TITLE = "Render Window";
	constexpr const char* VK_LAYER_KHRONOS_VALIDATION = "VK_LAYER_KHRONOS_validation";
	constexpr const char* CREATE_DEBUG_UTILS_MESSENGER_FUNC = "vkCreateDebugUtilsMessengerEXT";
	constexpr const char* DESTROY_DEBUG_UTILS_MESSENGER_FUNC = "vkDestroyDebugUtilsMessengerEXT";
	constexpr const char* APPLICATION_NAME = "Renderer";
	constexpr const char* ENGINE_NAME = "No Engine";
	constexpr const char* VERTEX_SHADER_PATH = "..\\shaders\\vert.spv";
	constexpr const char* FRAGMENT_SHADER_PATH = "..\\shaders\\frag.spv";
}

#endif