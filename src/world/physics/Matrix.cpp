//
// Created by diego on 24.02.18.
//


#include "Matrix.h"

Matrix::Matrix(double matrix[]) {
    xx = matrix[0];
    xy = matrix[1];
    xz = matrix[2];
    yx = matrix[3];
    yy = matrix[4];
    yz = matrix[5];
    zx = matrix[6];
    zy = matrix[7];
    zz = matrix[8];

}

Triple * Matrix::multiply_MT(Triple *triple) {
    double resultA[3] ;
    resultA[0] = xx * triple->x + xy * triple->y + xz * triple->z;
    resultA[1] = yx * triple->x + yy * triple->y + yz * triple->z;
    resultA[2] = zx * triple->x + zy * triple->y + zz * triple->z;
    Triple *resultT = new Triple(resultA);
    return resultT;

}

Matrix * Matrix::multiply_MM(Matrix *matrix) {
    double resultA[9]  ;

    resultA[0] = (xx * matrix->xx + xy * matrix->yx + xz * matrix->zx);
    resultA[1] = (xx * matrix->xy + xy * matrix->yy + xz * matrix->zy);
    resultA[2] = (xx * matrix->xz + xy * matrix->yz + xz * matrix->zz);
    resultA[3] = (yx * matrix->xx + yy * matrix->yx + yz * matrix->zx);
    resultA[4] = (yx * matrix->xy + yy * matrix->yy + yz * matrix->zy);
    resultA[5] = (yx * matrix->xz + yy * matrix->yz + yz * matrix->zz);
    resultA[6] = (zx * matrix->xx + zy * matrix->yx + zz * matrix->zx);
    resultA[7] = (zx * matrix->xy + zy * matrix->yy + zz * matrix->zy);
    resultA[8] = (zx * matrix->xz + zy * matrix->yz + zz * matrix->zz);
    Matrix *resultM = new Matrix(resultA);
    return resultM;

}

Matrix * Matrix::transpose() {
    Matrix *result = new Matrix();
    result->xy = yx;
    result->zy = yz;
    result->yx = xy;
    result->zx = xz;
    result->xz = zx;
    result->yz = zy;
    result->xx = xx;
    result->yy = yy;
    result->zz = zz;
    return result;
}

Matrix::Matrix() {
    xx = 1;
    xy = 0;
    xz = 0;
    yx = 0;
    yy = 1;
    yz = 0;
    zx = 0;
    zy = 0;
    zz = 1;
}

double * Matrix::byIndex(int i, int j) {
    if( i == 0){
        if( j == 0){
            return &xx;
        }
        if( j == 1){
            return &xy;
        }
        if( j == 2){
            return &xz;
        }
    }

    if( i == 1){
        if( j == 0){
            return &yx;
        }
        if( j == 1){
            return &yy;
        }
        if( j == 2){
            return &yz;
        }
    }

    if( i == 2){
        if( j == 0){
            return &zx;
        }
        if( j == 1){
            return &zy;
        }
        if( j == 2){
            return &zz;
        }
    }
}

Matrix * Matrix::multiply_MC(double c) {
    Matrix *result = new Matrix();
    for( int i = 0; i < 3; i++){
        for( int j = 0; j < 3; j++){
            *result->byIndex(i,j) = c * (*result->byIndex(i,j));

        }
    }
    return result;
}



