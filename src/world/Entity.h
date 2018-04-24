//
// Created by Milan van Zanten on 16.04.18.
//

#ifndef BESTEST_GAME_ENTITY_H
#define BESTEST_GAME_ENTITY_H

#include "../graphics/model/Model.h"
#include "physics/RigidBody.h"

class Entity {
public:
    void setModel(Model *model);
    Model *getModel() const;
    RigidBody *getRigidBody() const;
    virtual ~Entity();

private:
    Model *model;
    RigidBody *rigidBody;
};

#endif //BESTEST_GAME_ENTITY_H
