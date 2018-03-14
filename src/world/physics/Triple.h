//
// Created by diego on 02.03.18.
//

#ifndef BESTEST_GAME_TRIPLE_H
#define BESTEST_GAME_TRIPLE_H


#include "Matrix.h"
class Matrix;

class Triple {
public:
    Triple();
    Triple(double triple[]);
    Triple * division(double c);
    Matrix * star();

    double x;
    double y;
    double z;
private:

};


#endif //BESTEST_GAME_TRIPLE_H
