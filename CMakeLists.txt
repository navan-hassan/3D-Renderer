cmake_minimum_required(VERSION 3.29)

project(3DRenderer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Check if VULKAN_SDK environment variable is set
if(NOT DEFINED ENV{VULKAN_SDK})
    message(FATAL_ERROR "Vulkan SDK not found. Please install the Vulkan SDK and set the VULKAN_SDK environment variable.")
endif()


set(FETCHCONTENT_BASE_DIR ${CMAKE_SOURCE_DIR}/external)

include(FetchContent)

find_package(Vulkan REQUIRED)

FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG 3.4 
)
FetchContent_MakeAvailable(glfw)

FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG 1.0.1
)
FetchContent_MakeAvailable(glm)

set(SOURCES
    src/main.cpp
    src/renderer.cpp
)

set(HEADERS
    include/renderer.h
)

source_group("Source Files" FILES ${SOURCES})
source_group("Header Files" FILES ${HEADERS})

add_executable(3DRenderer ${SOURCES} ${HEADERS})

target_include_directories(3DRenderer PRIVATE 
    ${Vulkan_INCLUDE_DIRS}
    ${glm_SOURCE_DIR}
    ${glfw_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(3DRenderer PRIVATE 
    ${Vulkan_LIBRARIES}  # Vulkan loader library
    glfw                 # GLFW library
)

target_compile_definitions(3DRenderer PRIVATE
    GLFW_INCLUDE_VULKAN # Ensures GLFW includes Vulkan headers
)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT 3DRenderer)
