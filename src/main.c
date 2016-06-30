#include <stdio.h>
#include <stdlib.h>
#include "reader.h"
#include "hessenberg.h"
#include "qr.h"

#define PERCENT_EIGS 1

int main(int argc, char** argv){
  int i,j;
  int transpose = 0;
  PGMData* pgm = malloc(sizeof *pgm);

  pgm = readPGM("img/Rodney.pgm",pgm);

  Matrix *a,*at,*aat,*ata,*r1,*r2,*q1,*q2,*p1,*p2,*ut,*sigma,*v;
  Tridiagonal *aat2,*ata2;
  
  a = createMatrix(pgm->row,pgm->col);
  populateMatrix(a,pgm->matrix);

  at = createTranspose(a);
  
  if(a->row < a->col){
    transpose = 1;
    aat = a;
    a = at;
    at = a;
  }
  
  printf("Calculating A*At and AtA...\n");
  aat = multMatrix(a,at);
  ata = multMatrix(at,a);


  ///////////////////////////////////////////
  printf("Calculating hessenberg of A*At...\n");
  aat2 = toHessenberg(aat);  
  p1 = getP();
  printf("QR method for hess(A*At)...\n");
  qrMethod(aat2->sup,aat2->princ,aat2->sub);
  r1 = getR();
  q1 = getQ();
  ////////////////////////////////////////////
  printf("Calculating hessenberg of At*A...\n");
  ata2 = toHessenberg(ata);
  p2 = getP();
  printf("QR method for hess(At*A)...\n");
  qrMethod(ata2->sup,ata2->princ,ata2->sub);
  r2 = getR();
  q2 = getQ();
  ////////////////////////////////////////////

  for(i = 0; i < pgm->row; i++)
    for(j = 0; j < pgm->col; j++)
      if( i == j )
	r1->data[i][j] = sqrt(absF(r1->data[i][j]));

  ut = multMatrix(p2,q2);
  ut = createTranspose(sortMatrix(r2, ut));
  v = multMatrix(p1,q1);
  v = sortMatrix(r1,v);
  sigma = createMatrix(v->row,ut->col);
  for(i = 0; i < sigma->col; i++)
    sigma->data[i][i] = r1->data[i][i];

  /* Fixing signals and cut low eigs */

  Matrix* helper, *sigmaSigs;
  helper = multMatrix(at,v);
  sigmaSigs = multMatrix(ut, helper);
  for(i = 0; i < sigmaSigs->row && i < ut->row; i++){
    
    /* cut low eigen */
    if(i >= PERCENT_EIGS*ut->col) sigma->data[i][i] = 0;

    if(sigmaSigs->data[i][i] < 0){
      /* multiply i-th line of ut by -1*/
      for(j = 0; j < ut->col; j++) ut->data[i][j] *=-1;
    }
  }
  freeMatrix(helper);
  freeMatrix(sigmaSigs);


  /* End of fix signals */
  Matrix* aux2;
  Matrix* aux = multMatrix(sigma,ut);
  aux = multMatrix(v,aux);
  if(transpose){
    aux2 = aux;
    aux =  createTranspose(aux);
    free(aux2);      
  }

  PGMData* pgm2 = malloc(sizeof *pgm2);
  pgm2->row = pgm->row;
  pgm2->col = pgm->col;
  pgm2->max_gray = pgm->max_gray;
  pgm2->matrix = allocateDynamicMatrix(pgm->row,pgm->col);
  depopulateMatrix(pgm2->matrix,aux);

  writePGM("img/original.pgm",pgm);
  writePGM("img/answ.pgm",pgm2);

  free(pgm);
  freeMatrix(aux);

  return 0;
}
