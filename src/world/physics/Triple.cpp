//
// Created by diego on 02.03.18.
//

#include "Triple.h"

Triple::Triple(double triple[]) {
    x = triple[0];
    y = triple[1];
    z = triple[2];

}

Triple * Triple::division(double c) {
    Triple *result = new Triple();
    result->x = x/c;
    result->y = y/c;
    result->z = z/c;

    return result;

}

Triple::Triple() {
    x = 0;
    y = 0;
    z = 0;

}

Matrix * Triple::star() {

    Matrix *matrix = new Matrix();
    matrix->xx = 0;
    matrix->yy = 0;
    matrix->zz = 0;
    matrix->xy = z;
    matrix->xz = -y;
    matrix->yx = -z;
    matrix->yz = x;
    matrix->zx = y;
    matrix->zy = -x;

}
