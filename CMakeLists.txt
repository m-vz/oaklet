cmake_minimum_required(VERSION 3.8)
project(bestest-game)

set(CMAKE_CXX_STANDARD 11)

# flags
SET(COMPILE_FLAGS "")
SET(LINK_FLAGS "")
SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${COMPILE_FLAGS}")
SET(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} ${LINK_FLAGS}")

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

    # glm
    set(INCLUDE_DIRS ${INCLUDE_DIRS} lib/glm)
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
    add_definitions(-DGLEW_STATIC)
    option(glew-cmake_BUILD_SHARED "" OFF)
    add_subdirectory(lib/glew)

    set(INCLUDE_DIRS ${INCLUDE_DIRS} lib/glew/include lib/glew/src)
    set(LIBS ${LIBS} ${GLEW_LIBRARY})

    # glm
    set(INCLUDE_DIRS ${INCLUDE_DIRS} lib/glm)
endif(${CMAKE_SYSTEM_NAME} MATCHES "Linux")

# assets
file(GLOB_RECURSE assets RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} assets/*)
foreach(asset ${assets})
    message(${asset})
    configure_file(${asset} ${CMAKE_CURRENT_BINARY_DIR}/${asset} COPYONLY)
endforeach()

set(SOURCE_FILES src/world/Time.cpp src/world/Time.h src/io/FileLoader.cpp src/io/FileLoader.h src/io/IOControl.cpp src/io/IOControl.h src/io/Mouse.cpp src/io/Mouse.h src/util/Log.cpp src/util/Log.h src/util/Config.h src/exception/Exception.cpp src/exception/Exception.h src/main.cpp src/world/World.cpp src/world/World.h src/graphics/Renderer.cpp src/graphics/Renderer.h src/io/Keyboard.cpp src/io/Keyboard.h)
add_executable(bestest-game ${SOURCE_FILES})

include_directories(${INCLUDE_DIRS})
target_link_libraries(bestest-game ${LIBS})