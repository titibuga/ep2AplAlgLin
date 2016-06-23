#include "vector.h"

void multByScalar(float alpha, Vector* vec){
  int i;
  
  for(i = 0; i < vec->len; i++) vec->data[i] *= alpha;
}

void addVectors(Vector* A, Vector* B, Vector* C){
  int i;

  for(i = 0; i < A->len; i++) C->data[i] = A->data[i] + B->data[i];
}

void subVectors(Vector* A, Vector* B, Vector* C){
  int i;

  for(i = 0; i < A->len; i++) C->data[i] = A->data[i] - B->data[i];
}

float dotProduct(Vector* A, Vector* B){
  int i;
  float c;
  
  for(i = 0; i < A->len; i++) c += A->data[i] * B->data[i];
  return c;
}

Vector* createVector(int len){
  Vector* aux = malloc(sizeof *aux);
  aux->len = len;
  aux->data = calloc(aux->len , sizeof(float));
  return aux;
}

void freeVector(Vector* vec){
  free(vec->data);
  free(vec);
}


