//
// Created by diego on 24.02.18.
//

#ifndef BESTEST_GAME_RIGIDBODY_H
#define BESTEST_GAME_RIGIDBODY_H


#include <glm/detail/type_mat.hpp>
#include "Matrix.h"
#include "Triple.h"
#include "../../graphics/model/Model.h"

#define STATE_SIZE 18


class RigidBody  {

public:
    explicit RigidBody(Model *model);
    void State_to_Array(double y[]);
    void Array_to_State(double y[]);
    void Compute_Force_and_Torque(double t);
    void ddt_State_to_Array(double *ydot);
    void initCube(double m);
    void adjustModel();

    Triple *x;


private:
    Model *model;

    /* Constant quantities */
    double mass;
    /* mass M */
    Matrix *Ibody, *Ibodyinv;
    /* I body */

    /* State variables */

    Matrix *R;
    Triple *L;
    Triple *P;
    /*x(t). R(t), P(t), L(t) */

    /* Derived quantities (auxiliary variables) */
    Matrix *Iinv;
    /* I −1 (t) */
    Triple *v;
    /* I −1 (t) */
    Triple *omega;
    /* v(t), ω(t) */

    /* Computed quantities */
    Triple *force, *torque;
    /* F(t), τ(t) */
};


#endif //BESTEST_GAME_RIGIDBODY_H
