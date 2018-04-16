//
// Created by diego on 12.02.18.
//

#ifndef BESTEST_GAME_PHYSICS_H
#define BESTEST_GAME_PHYSICS_H

#include <vector>
#include "PhysicsObject.h"
#include "RigidBody.h"

class Physics {
public:
    void update(long long int deltaTime);
    void addPhysicsObject(RigidBody* objectToAdd);
    void removePhysicsObject(RigidBody* objectToRemove);
    void Array_to_Bodies(double y[]);
    void Bodies_to_Array(double y[]);
    void dydt(long long int deltaTime, double y[], double ydot[]);

    double *y0;
    double *yfinal;
    int nBodies = 0;
    int stateSize = 18;
    int c = 1;

private:
    std::vector<RigidBody*> rigidBodies{};
};


#endif //BESTEST_GAME_PHYSICS_H
