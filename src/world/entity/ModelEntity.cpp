//
// Created by Milan van Zanten on 06.05.18.
//

#include "ModelEntity.h"

void ModelEntity::setModel(Model *model) {
    this->model = model;
    rigidBody = new RigidBody(model);
}

Model *ModelEntity::getModel() const {
    return model;
}

RigidBody *ModelEntity::getRigidBody() const {
    return rigidBody;
}

ModelEntity::~ModelEntity() {
    delete rigidBody;
}