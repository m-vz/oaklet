//
// Created by Milan van Zanten on 16.04.18.
//

#include "Entity.h"

void Entity::setModel(Model *model) {
    this->model = model;
    rigidBody = new RigidBody(model);
}

Model *Entity::getModel() const {
    return model;
}

RigidBody *Entity::getRigidBody() const {
    return rigidBody;
}
