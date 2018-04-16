//
// Created by diego on 24.02.18.
//

#include "RigidBody.h"

void RigidBody::State_to_Array(double y[]) {

    y[0] = x->x;
    y[1] = x->y;
    y[2] = x->z;

    for( int i = 0; i < 3; i++){
        for( int j = 0; j < 3; j++){
            y[i*3 + j + 3] = *(R->byIndex(i,j));
        }
    }


    y[12] = P->x;
    y[13] = P->y;
    y[14] = P->z;

    y[15] = L->x;
    y[16] = L->y;
    y[17] = L->z;


}



void RigidBody::Array_to_State(double y[]) {
    x->x = y[0];
    x->y = y[1];
    x->z = y[2];

    for( int i = 0; i < 3; i++){
        for( int j = 0; j < 3; j++){
            *(R->byIndex(i,j)) = y[i*3 + j + 3];
        }
    }

    P->x = y[12];
    P->y = y[13];
    P->z = y[14];

    L->x = y[15];
    L->y = y[16];
    L->z = y[17];

    v = P->division(mass);

    Iinv = R->multiply_MM(Ibodyinv)->multiply_MM(R->transpose());

    omega = Iinv->multiply_MT(L);
}

void RigidBody::Compute_Force_and_Torque(double t) {
    if(t == 0) {
        double forceA[3];
        forceA[0] = 9.81;
        forceA[1] = 9.81;
        forceA[2] = 0;
        force = new Triple(forceA);
        torque = new Triple(forceA);
    } else {
        double forceA[3];
        forceA[0] = 9.81;
        forceA[1] = 0;
        forceA[2] = 0;
        force = new Triple(forceA);
        torque = new Triple(forceA);
    }
}

void RigidBody::ddt_State_to_Array(double ydot[]) {
    ydot[0] = v->x;
    ydot[1] = v->y;
    ydot[2] = v->z;

    Matrix *Rdot = R->multiply_MM(omega->star());

    for(int i = 0; i < 3; i++){
        for(int j = 0; i < 3; i++){
            ydot[i*3 + j + 3] = *(Rdot->byIndex(i,j));
        }
    }

    ydot[12] = force->x;
    ydot[13] = force->y;
    ydot[14] = force->z;

    ydot[15] = torque->x;
    ydot[16] = torque->y;
    ydot[17] = torque->z;









}

void RigidBody::initCube(double mass) {
    this->mass = mass;
    Ibody = new Matrix();
    Ibody = Ibody->multiply_MC(1./6);
    Ibodyinv = Ibody->multiply_MC(36);
    x = new Triple();
    P = new Triple();
    L = new Triple();
    R = new Matrix();

    v = P->division(mass);

    Iinv = R->multiply_MM(Ibodyinv)->multiply_MM(R->transpose());

    omega = Iinv->multiply_MT(L);




}

RigidBody::RigidBody(Model *model) {
    modelMatrix = &model->modelMatrix;
    initCube(1);
}
