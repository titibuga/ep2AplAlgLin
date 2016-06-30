#include <stdio.h>
#include <stdlib.h>
#include "reader.h"
#include "hessenberg.h"
#include "qr.h"

int main(int argc, char** argv){
  int i,j;
  PGMData* pgm = malloc(sizeof *pgm);

  pgm = readPGM("img/hue.pgm",pgm);

  Matrix *a,*at,*aat,*ata,*r1,*r2,*q1,*q2,*p1,*p2,*ut,*sigma,*v;
  Tridiagonal *aat2,*ata2;
  
  a = createMatrix(pgm->row,pgm->col);
  populateMatrix(a,pgm->matrix);

  at = createTranspose(a);
  aat = multMatrix(a,at);
  ata = multMatrix(at,a);

  ///////////////////////////////////////////
  aat2 = toHessenberg(aat);
  p1 = getP();
  qrMethod(aat2->sup,aat2->princ,aat2->sub);
  r1 = getR();
  q1 = getQ();
  ////////////////////////////////////////////
  ata2 = toHessenberg(ata);
  p2 = getP();
  qrMethod(ata2->sup,ata2->princ,ata2->sub);
  r2 = getR();
  q2 = getQ();
  ////////////////////////////////////////////

  for(i = 0; i < pgm->row; i++){
    for(j = 0; j < pgm->col; j++){
      if( i == j ){
	r1->data[i][j] = sqrt(r1->data[i][j]);
	r2->data[i][j] = sqrt(r2->data[i][j]);
      }
    }
  }

  ut = createTranspose(multMatrix(p2,q2));
  v = multMatrix(p1,q1);
  sigma = r1;

  Matrix* aux = multMatrix(sigma,ut);
  aux = multMatrix(v,aux);
  

  PGMData* pgm2 = malloc(sizeof *pgm2);
  pgm2->row = pgm->row;
  pgm2->col = pgm->col;
  pgm2->max_gray = pgm->max_gray;
  pgm2->matrix = allocateDynamicMatrix(pgm->row,pgm->col);
  depopulateMatrix(pgm2->matrix,aux);

  writePGM("img/hue.pgm",pgm);
  writePGM("img/bla.pgm",pgm2);

  free(pgm);

  return 0;
}
