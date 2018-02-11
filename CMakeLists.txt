cmake_minimum_required(VERSION 3.8)
project(bestest-game)

set(CMAKE_CXX_STANDARD 11)

if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    # Mac OS X specific code

    # glfw
    set(GLFW_BUILD_EXAMPLES OFF CACHE STRING "" FORCE)
    set(GLFW_BUILD_TESTS OFF CACHE STRING "" FORCE)
    set(GLFW_INSTALL OFF CACHE STRING "" FORCE)
    add_subdirectory(lib/glfw)
    include_directories(lib/glfw/include)

    # glew
    add_definitions(-DGLEW_STATIC)
    option(glew-cmake_BUILD_SHARED "" OFF)
    add_subdirectory(lib/glew)
    include_directories(lib/glew/include lib/glew/src)

    # glm
    include_directories(lib/glm)
endif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")

# assets
file(GLOB_RECURSE assets RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} assets/*)
foreach(asset ${assets})
    message(${asset})
    configure_file(${asset} ${CMAKE_CURRENT_BINARY_DIR}/${asset} COPYONLY)
endforeach()

set(SOURCE_FILES src/world/Time.cpp src/world/Time.h src/io/FileLoader.cpp src/io/FileLoader.h src/io/IOControl.cpp src/io/IOControl.h src/io/Mouse.cpp src/io/Mouse.h src/util/Log.cpp src/util/Log.h src/util/Config.h src/exception/Exception.cpp src/exception/Exception.h src/main.cpp src/world/World.cpp src/world/World.h src/graphics/Renderer.cpp src/graphics/Renderer.h src/io/Keyboard.cpp src/io/Keyboard.h)
add_executable(bestest-game ${SOURCE_FILES})
target_link_libraries(bestest-game glfw ${GLFW_LIBRARIES} libglew_static)