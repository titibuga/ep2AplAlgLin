#ifndef VECTOR_H
#define VECTOR_H

#include <stdlib.h>

typedef struct vec{
  int len;
  float* data;
}Vector;

void multByScalar(float, Vector*);
void addVectors(Vector*, Vector*, Vector*);
void subVectors(Vector*, Vector*, Vector*);
float dotProduct(Vector*, Vector*);
Vector* createVector(int);
void freeVector(Vector*);

#endif
