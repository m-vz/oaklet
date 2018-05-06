//
// Created by Milan van Zanten on 06.05.18.
//

#ifndef BESTEST_GAME_MODELENTITY_H
#define BESTEST_GAME_MODELENTITY_H

#include "../../graphics/model/Model.h"
#include "../physics/RigidBody.h"

class ModelEntity {
public:
    virtual void setModel(Model *model);
    virtual Model *getModel() const;
    virtual RigidBody *getRigidBody() const;
    virtual ~ModelEntity();

private:
    Model *model = nullptr;
    RigidBody *rigidBody = nullptr;
};

#endif //BESTEST_GAME_MODEL// ENTITY_H
