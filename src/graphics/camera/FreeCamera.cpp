//
// Created by Milan van Zanten on 16.03.18.
//

#define GLM_ENABLE_EXPERIMENTAL

#include "FreeCamera.h"
#include "../../util/Log.h"

FreeCamera::FreeCamera(Window &capturedIn, Mouse &mouse, Keyboard &keyboard) : capturedIn(capturedIn) {
    changeAspect(capturedIn.getWindowWidth(), capturedIn.getWindowHeight());
    update(0);

    mouse.capture(capturedIn);
    mouse.addMoveCallback([this](Mouse &mouse) {
        angles.x += glm::radians<float>(static_cast<float>(mouse.position().y) / 6.0f);
        angles.y += glm::radians<float>(static_cast<float>(mouse.position().x) / 6.0f);
    });
    keyboard.addPressedCallback([this](Keyboard &keyboard, int key, int scancode, int mods) {
        acceleration.x = speed;
    }, GLFW_KEY_W);
    keyboard.addReleasedCallback([this](Keyboard &keyboard, int key, int scancode, int mods) {
        if(keyboard.keyPressed(this->capturedIn.getGLFWWindow(), GLFW_KEY_S))
            acceleration.x = -speed;
        else
            acceleration.x = 0;
    }, GLFW_KEY_W);
    keyboard.addPressedCallback([this](Keyboard &keyboard, int key, int scancode, int mods) {
        acceleration.x = -speed;
    }, GLFW_KEY_S);
    keyboard.addReleasedCallback([this](Keyboard &keyboard, int key, int scancode, int mods) {
        if(keyboard.keyPressed(this->capturedIn.getGLFWWindow(), GLFW_KEY_W))
            acceleration.x = speed;
        else
            acceleration.x = 0;
    }, GLFW_KEY_S);

    keyboard.addPressedCallback([this](Keyboard &keyboard, int key, int scancode, int mods) {
        acceleration.y = speed;
    }, GLFW_KEY_D);
    keyboard.addReleasedCallback([this](Keyboard &keyboard, int key, int scancode, int mods) {
        if(keyboard.keyPressed(this->capturedIn.getGLFWWindow(), GLFW_KEY_A))
            acceleration.y = -speed;
        else
            acceleration.y = 0;
    }, GLFW_KEY_D);
    keyboard.addPressedCallback([this](Keyboard &keyboard, int key, int scancode, int mods) {
        acceleration.y = -speed;
    }, GLFW_KEY_A);
    keyboard.addReleasedCallback([this](Keyboard &keyboard, int key, int scancode, int mods) {
        if(keyboard.keyPressed(this->capturedIn.getGLFWWindow(), GLFW_KEY_D))
            acceleration.y = speed;
        else
            acceleration.y = 0;
    }, GLFW_KEY_A);

    keyboard.addPressedCallback([this](Keyboard &keyboard, int key, int scancode, int mods) {
        acceleration.z = speed;
    }, GLFW_KEY_SPACE);
    keyboard.addReleasedCallback([this](Keyboard &keyboard, int key, int scancode, int mods) {
        if(keyboard.keyPressed(this->capturedIn.getGLFWWindow(), GLFW_KEY_LEFT_SHIFT))
            acceleration.z = -speed;
        else
            acceleration.z = 0;
    }, GLFW_KEY_SPACE);
    keyboard.addPressedCallback([this](Keyboard &keyboard, int key, int scancode, int mods) {
        acceleration.z = -speed;
    }, GLFW_KEY_LEFT_SHIFT);
    keyboard.addReleasedCallback([this](Keyboard &keyboard, int key, int scancode, int mods) {
        if(keyboard.keyPressed(this->capturedIn.getGLFWWindow(), GLFW_KEY_SPACE))
            acceleration.z = speed;
        else
            acceleration.z = 0;
    }, GLFW_KEY_LEFT_SHIFT);
}

void FreeCamera::changeAspect(int width, int height) {
    // projection matrix: 45° Field of View, display range : 0.1 unit <-> 1000 units
    projection = glm::perspective(glm::radians(45.0f), (float) width / (float) height, 0.1f, 1000.0f);
}

void FreeCamera::update(long long int time) {
    lookingDirection = glm::normalize(glm::rotateY(glm::rotateX(glm::vec3(0.0f, 0.0f, 1.0f), -angles.x), -angles.y));
    rightDirection = glm::normalize(glm::cross<glm::highp_float, glm::highp>(lookingDirection, upDirection));

    float seconds = time/1000000000.0f;
    accelerationTmp = acceleration*seconds;
    velocity += lookingDirection*accelerationTmp.x + rightDirection*accelerationTmp.y + upDirection*accelerationTmp.z - velocity*friction;
    position += velocity*seconds;

    view = glm::lookAt(position, position + lookingDirection, upDirection);
}
