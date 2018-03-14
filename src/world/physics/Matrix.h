//
// Created by diego on 24.02.18.
//

#ifndef BESTEST_GAME_MATRIX_H
#define BESTEST_GAME_MATRIX_H

#include "Triple.h"
class Triple;

class Matrix {
public:
    Matrix();
    Matrix(double matrix[]);
    Matrix * multiply_MM(Matrix *matrix);
    Triple * multiply_MT(Triple *triple);
    Matrix * multiply_MC(double c);
    Matrix * transpose();
    double * byIndex(int i, int j);

    double yx;
    double xz;
    double xy;
    double xx;
    double yy;
    double yz;
    double zx;
    double zy;
    double zz;

};


#endif //BESTEST_GAME_MATRIX_H
