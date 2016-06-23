#include "vector.h"

void multByScalar(float alpha, Vector* vec){
  int i;
  
  for(i = 0; i < vec->len; i++) vec->vector[i] *= alpha;
}

void addVectors(Vector* A, Vector* B, Vector* C){
  int i;

  for(i = 0; i < A->len; i++) C->vector[i] = A->vector[i] + B->vector[i];
}

void subVectors(Vector* A, Vector* B, Vector* C){
  int i;

  for(i = 0; i < A->len; i++) C->vector[i] = A->vector[i] - B->vector[i];
}

float dotProduct(Vector* A, Vector* B){
  int i;
  float c;
  
  for(i = 0; i < A->len; i++) c += A->vector[i] * B->vector[i];
  return c;
}

Vector* createVector(int len){
  Vector* aux = malloc(sizeof *aux);
  aux->vector = calloc(aux->len , sizeof(float));
  return aux;
}

void freeVector(Vector* vec){
  free(vec->vector);
  free(vec);
}


