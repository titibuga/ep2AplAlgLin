#ifndef QR_H
#define QR_H

#include <math.h>
#include <stdlib.h>
#include "vector.h"
#include "matrix.h"

typedef struct givens{
  float sin;
  float cos;
}Givens;

void qrDecomposition(Vector*,Vector*,Vector*);
Givens* givensRotation(float,float);
void applyLeftGivensRotation(Givens*, Matrix*);


#endif 
