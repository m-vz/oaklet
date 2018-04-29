//
// Created by Milan van Zanten on 16.04.18.
//

#ifndef BESTEST_GAME_TECHNIQUE_H
#define BESTEST_GAME_TECHNIQUE_H

#include <GL/glew.h>
#include <string>
#include <vector>

class Technique {
public:
    Technique();
    virtual void init();
    void enable();
    void validate();
    virtual void execute() = 0;
    virtual ~Technique();

protected:
    void addShader(GLenum shaderType, std::string shaderPath);
    void finalize();
    GLuint getUniformLocation(std::string uniformName);
    GLint getProgramParam(GLenum param);
    GLuint shaderProgram;

private:
    bool initialized = false;
    std::vector<GLuint> shaderObjects;
};

#endif //BESTEST_GAME_TECHNIQUE_H
