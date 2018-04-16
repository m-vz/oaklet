//
// Created by diego on 12.02.18.
//

#include <algorithm>
#include "Physics.h"
#include "../../util/Log.h"

void Physics::update(long long int deltaTime) {
    for(int i = 0; i < stateSize * nBodies; i++)
        y0[i] = yfinal[i];
    dydt(deltaTime, y0, yfinal);
    for(int i = 0; i < stateSize * nBodies; i++){

        yfinal[i] = y0[i] + yfinal[i]*(deltaTime/1000000000.0);
    }
    Array_to_Bodies(yfinal);

    for(int i = 0; i < nBodies; ++i)
        rigidBodies[i]->adjustModel();

    if(!rigidBodies.empty()) {
        Log::log << LOG_DEBUG << "x " << rigidBodies[0]->x->x;
        Log::log << LOG_DEBUG << "y " << rigidBodies[0]->x->y;
    }
}

void Physics::addPhysicsObject(RigidBody *objectToAdd) {
    rigidBodies.push_back(objectToAdd);

    nBodies++;
    y0 = new double[nBodies*stateSize]{};
    yfinal = new double[nBodies*stateSize]{};
    Bodies_to_Array(yfinal);
    for(int i = 0; i < stateSize * nBodies; i++)
        y0[i] = yfinal[i];
}

void Physics::removePhysicsObject(RigidBody *objectToRemove) {
    auto i = std::find(rigidBodies.begin(), rigidBodies.end(), objectToRemove);
    if(i != rigidBodies.end()) {
        rigidBodies.erase(i);
    }

    nBodies--;
    y0 = new double[nBodies*stateSize]{};
    yfinal = new double[nBodies*stateSize]{};
    Bodies_to_Array(yfinal);
    for(int j = 0; j < stateSize * nBodies; j++)
        y0[j] = yfinal[j];
}

void Physics::Array_to_Bodies(double *y) {
    for(int i = 0; i < nBodies; i++){
        rigidBodies[i]->Array_to_State(&y[i*stateSize]);
    }

}

void Physics::Bodies_to_Array(double *y) {
    for(int i = 0; i < nBodies; i++){
        rigidBodies[i]->State_to_Array(&y[i*stateSize]);
    }
}

void Physics::dydt(long long int deltaTime, double y[], double ydot[]) {
    Array_to_Bodies(y);

    for(int i = 0; i < nBodies; i++){
        rigidBodies[i]->Compute_Force_and_Torque(deltaTime);
        rigidBodies[i]->ddt_State_to_Array(&ydot[i*STATE_SIZE]);
    }
}

