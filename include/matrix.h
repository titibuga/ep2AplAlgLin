#ifndef UTILS_H
#define UTILS_H

#include <stdlib.h>
#include <stdio.h>

typedef struct mat{
  int row;
  int col;
  float** data;
}Matrix;

float** allocateMatrix(int,int);
Matrix* createMatrix(int,int);
void freeMatrix(Matrix*);
void populateMatrix(Matrix*,int**);
Matrix* multMatrix(Matrix*,Matrix*);
void deallocateMatrix(float**,int);

#endif
