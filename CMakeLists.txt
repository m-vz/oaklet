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

# stb
set(INCLUDE_DIRS ${INCLUDE_DIRS} lib/stb)

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

    # assimp
    set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
    set(ASSIMP_BUILD_ASSIMP_TOOLS OFF CACHE BOOL "" FORCE)
    set(ASSIMP_BUILD_STATIC_LIB ON CACHE BOOL "" FORCE)
    add_subdirectory(lib/assimp)

    set(INCLUDE_DIRS ${INCLUDE_DIRS} lib/assimp/include ${CMAKE_CURRENT_BINARY_DIR}/lib/assimp/include lib/assimp/code)
    set(LIBS ${LIBS} assimp)
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

    # assimp
    set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
    set(ASSIMP_BUILD_ASSIMP_TOOLS OFF CACHE BOOL "" FORCE)
    set(ASSIMP_BUILD_STATIC_LIB ON CACHE BOOL "" FORCE)
    add_subdirectory(lib/assimp)

    set(INCLUDE_DIRS ${INCLUDE_DIRS} lib/assimp/include ${CMAKE_CURRENT_BINARY_DIR}/lib/assimp/include lib/assimp/code)
    set(LIBS ${LIBS} assimp)
endif(${CMAKE_SYSTEM_NAME} MATCHES "Linux")

# assets
#set(ASSET_DIR ${CMAKE_CURRENT_SOURCE_DIR}/assets/)
#file(GLOB_RECURSE ASSET_FILES ${ASSET_DIR}*.obj ${ASSET_DIR}*.mtl ${ASSET_DIR}*.3ds ${ASSET_DIR}*.glsl ${ASSET_DIR}*.png ${ASSET_DIR}*.jpg ${ASSET_DIR}*.bmp ${ASSET_DIR}*.exr ${ASSET_DIR}*.tga)
#foreach(ASSET_FILE ${ASSET_FILES})
#    file(RELATIVE_PATH ASSET_RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${ASSET_FILE})
#    get_filename_component(ASSET_RELATIVE_DIRECTORY ${ASSET_RELATIVE} DIRECTORY)
#    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/${ASSET_RELATIVE} ${CMAKE_CURRENT_BINARY_DIR}/${ASSET_RELATIVE} COPYONLY)
#endforeach()

set(SOURCE_FILES src/world/Time.cpp src/world/Time.h src/io/FileLoader.cpp src/io/FileLoader.h src/io/IOControl.cpp src/io/IOControl.h src/io/Mouse.cpp src/io/Mouse.h src/util/Log.cpp src/util/Log.h src/util/Config.h src/exception/Exception.cpp src/exception/Exception.h src/world/World.cpp src/world/World.h src/graphics/Renderer.cpp src/graphics/Renderer.h src/io/Keyboard.cpp src/io/Keyboard.h src/graphics/model/Mesh.cpp src/graphics/model/Mesh.h src/io/Window.cpp src/io/Window.h src/graphics/camera/Camera.cpp src/graphics/camera/Camera.h src/graphics/camera/CirclingCamera.cpp src/graphics/camera/CirclingCamera.h src/graphics/texture/Texture.cpp src/graphics/texture/Texture.h src/graphics/camera/FreeCamera.cpp src/graphics/camera/FreeCamera.h src/graphics/gui/font/BitmapFont.cpp src/graphics/gui/font/BitmapFont.h src/graphics/model/Model.cpp src/graphics/model/Model.h src/graphics/model/MeshFactory.cpp src/graphics/model/MeshFactory.h src/world/physics/Physics.cpp src/world/physics/Physics.h src/world/physics/PhysicsObject.cpp src/world/physics/PhysicsObject.h src/world/physics/RigidBody.cpp src/world/physics/RigidBody.h src/world/physics/Matrix.cpp src/world/physics/Matrix.h src/world/physics/Triple.cpp src/world/physics/Triple.h src/world/Scene.cpp src/world/Scene.h src/graphics/light/PointLight.cpp src/graphics/light/PointLight.h src/graphics/technique/Technique.cpp src/graphics/technique/Technique.h src/graphics/technique/SimpleLightingTechnique.cpp src/graphics/technique/SimpleLightingTechnique.h src/graphics/light/DirectionalLight.cpp src/graphics/light/DirectionalLight.h src/graphics/light/SpotLight.cpp src/graphics/light/SpotLight.h src/graphics/framebuffer/Framebuffer.cpp src/graphics/framebuffer/Framebuffer.h src/graphics/texture/DepthMapTexture.cpp src/graphics/texture/DepthMapTexture.h src/graphics/technique/ShadowMapTechnique.cpp src/graphics/technique/ShadowMapTechnique.h src/graphics/light/LightType.h src/graphics/light/LightWithShadowMap.cpp src/graphics/light/LightWithShadowMap.h src/graphics/texture/CubeTexture.cpp src/graphics/texture/CubeTexture.h src/world/Skybox.cpp src/world/Skybox.h src/graphics/technique/SkyboxTechnique.cpp src/graphics/technique/SkyboxTechnique.h src/graphics/texture/DepthMapCubeTexture.cpp src/graphics/texture/DepthMapCubeTexture.h src/BestestGameEngine.cpp src/BestestGameEngine.h src/world/entity/UpdateEntity.h src/world/entity/ModelEntity.cpp src/world/entity/ModelEntity.h src/graphics/light/MovingPointLight.cpp src/graphics/light/MovingPointLight.h src/graphics/texture/HDRTexture.cpp src/graphics/texture/HDRTexture.h src/graphics/texture/HDRCubeTexture.cpp src/graphics/texture/HDRCubeTexture.h src/graphics/technique/ToneMappingTechnique.cpp src/graphics/technique/ToneMappingTechnique.h src/graphics/texture/DepthStencilTexture.cpp src/graphics/texture/DepthStencilTexture.h src/graphics/technique/BitmapFontTechnique.cpp src/graphics/technique/BitmapFontTechnique.h src/graphics/gui/TextElement.cpp src/graphics/gui/TextElement.h)
add_executable(bestest-game ${SOURCE_FILES})

include_directories(${INCLUDE_DIRS})
target_link_libraries(bestest-game ${LIBS})