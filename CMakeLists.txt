cmake_minimum_required(VERSION 3.8)
project(bestest-game)

set(CMAKE_CXX_STANDARD 11)

# flags
SET(COMPILE_FLAGS "")
SET(LINK_FLAGS "")
SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${COMPILE_FLAGS}")
SET(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} ${LINK_FLAGS}")

# glm
set(INCLUDE_DIRS ${INCLUDE_DIRS} lib/glm)

# tinyobjloader
set(INCLUDE_DIRS ${INCLUDE_DIRS} lib/tinyobjloader)

# macos specific code
if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    # glfw
    set(GLFW_BUILD_EXAMPLES OFF CACHE STRING "" FORCE)
    set(GLFW_BUILD_TESTS OFF CACHE STRING "" FORCE)
    set(GLFW_INSTALL OFF CACHE STRING "" FORCE)
    add_subdirectory(lib/glfw)

    set(INCLUDE_DIRS ${INCLUDE_DIRS} lib/glfw/include)
    set(LIBS ${LIBS} glfw)

    # glew
    add_definitions(-DGLEW_STATIC)
    option(glew-cmake_BUILD_SHARED "" OFF)
    add_subdirectory(lib/glew)

    set(INCLUDE_DIRS ${INCLUDE_DIRS} lib/glew/include lib/glew/src)
    set(LIBS ${LIBS} libglew_static)
endif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")

# linux specific code
if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    # opengl
    find_package(OpenGL)

    set(LIBS ${LIBS} ${OPENGL_LIBRARY})

    # glfw
    set(GLFW_BUILD_EXAMPLES OFF CACHE STRING "" FORCE)
    set(GLFW_BUILD_TESTS OFF CACHE STRING "" FORCE)
    set(GLFW_INSTALL OFF CACHE STRING "" FORCE)
    add_subdirectory(lib/glfw)

    set(INCLUDE_DIRS ${INCLUDE_DIRS} lib/glfw/include)
    set(LIBS ${LIBS} glfw)

    # glew
    find_package(GLEW REQUIRED)

    set(INCLUDE_DIRS ${INCLUDE_DIRS} lib/glew/include lib/glew/src)
    set(LIBS ${LIBS} ${GLEW_LIBRARY})
endif(${CMAKE_SYSTEM_NAME} MATCHES "Linux")

# assets
file(COPY assets DESTINATION ${CMAKE_CURRENT_BINARY_DIR} FILES_MATCHING REGEX ".*\\.(obj|mtl|glsl)$")

set(SOURCE_FILES src/world/Time.cpp src/world/Time.h src/io/FileLoader.cpp src/io/FileLoader.h src/io/IOControl.cpp src/io/IOControl.h src/io/Mouse.cpp src/io/Mouse.h src/util/Log.cpp src/util/Log.h src/util/Config.h src/exception/Exception.cpp src/exception/Exception.h src/main.cpp src/world/World.cpp src/world/World.h src/graphics/Renderer.cpp src/graphics/Renderer.h src/io/Keyboard.cpp src/io/Keyboard.h src/graphics/Mesh.cpp src/graphics/Mesh.h src/io/Window.cpp src/io/Window.h src/graphics/camera/Camera.cpp src/graphics/camera/Camera.h src/graphics/camera/CirclingCamera.cpp src/graphics/camera/CirclingCamera.h src/world/physics/Physics.cpp src/world/physics/Physics.h src/world/physics/PhysicsObject.cpp src/world/physics/PhysicsObject.h src/world/physics/RigidBody.cpp src/world/physics/RigidBody.h src/world/physics/Matrix.cpp src/world/physics/Matrix.h src/world/physics/Triple.cpp src/world/physics/Triple.h)
add_executable(bestest-game ${SOURCE_FILES})

include_directories(${INCLUDE_DIRS})
target_link_libraries(bestest-game ${LIBS})