#include <stdio.h>
#include <stdlib.h>
#include "reader.h"

int main(int argc, char** argv){
  int i = 0;  
  int j = 0;
  PGMData* pgm = malloc(sizeof *pgm);

  pgm = readPGM("img/lena.pgm",pgm);
  /*
  for(i = 0; i < 512; i++){
    for(j = 0; j < 512; j++){
      if(pgm->matrix[i][j] > 127)
	pgm->matrix[i][j] = 255;
      else pgm->matrix[i][j] = 0;
    }
  }
  */
  writePGM("img/bla.pgm",pgm);
  
  return 0;
}
