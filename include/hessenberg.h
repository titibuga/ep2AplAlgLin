#ifndef HESSEMBERG_H
#define HESSEMBERG_H 1

#include "vector.h"

#define DEBUG_HESS 0

typedef struct tridiagonal_str{
  /*len_sub = len_sup = len_princ - 1*/
  int len_princ;
  Vector *princ, *sub, *sup;
}Tridiagonal;

#include "matrix.h"


Tridiagonal* toHessenberg(Matrix*);
Tridiagonal* createTridiagonal(int);
void freeTridiagonal(Tridiagonal*);
Vector* findW(Vector*, int);
void makeMatrixHouseHolder(Vector*, Matrix*);
void applyingLeftHouseHolder(Vector*, Vector**, int );
void applyingRightHouseHolder(Vector*, Vector**, int );
Matrix* getP();





#endif
