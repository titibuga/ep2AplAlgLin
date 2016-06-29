#include <stdio.h>
#include <stdlib.h>
#include "reader.h"
#include "qr.h"

int main(int argc, char** argv){
  int i = 0;  
  int j = 0;
  Vector *up,*mid,*low;
  PGMData* pgm = malloc(sizeof *pgm);

  pgm = readPGM("img/lena.pgm",pgm);
  writePGM("img/bla.pgm",pgm);

  up = createVector(4);
  mid = createVector(5);
  low = createVector(4);

  for(i = 0; i < 4; i++){
    up->data[i] = mid->data[i] = low->data[i] = 1;
  }
  mid->data[4] = 1;

  qrMethod(up,mid,low);
  
  return 0;
}
