
#include <math.h>
#include "vector.h"
#include "matrix.h"
#include "hessenberg.h"


Matrix *P;


/*****************************************************************************/
/* toHessenberg:                                                             */
/* Assume-se que receberá uma matriz A real simetrica => forma de Hessenberg */
/* é tridiagonal                                                             */
/* A_n[i] => coluna i de A                                                   */
/* W[i]   => vetor w para reflexão de Househouder vinculado à coluna i       */
/*****************************************************************************/
Tridiagonal* toHessenberg(Matrix* A){
  int i, j, k;
  Tridiagonal *H;
  Vector **W;
  Vector **A_n;
  Matrix *Aux, *Aux2;

  P = createMatrix(A->row, A->col);
  Aux = createMatrix(A->row, A->col);

  for(i = 0; i < A->row; i++) P->data[i][i] = 1;

  W = malloc((A->col - 2) * sizeof(*W));
  /*criando/inicializando A_n tq A_n = A*/
  A_n = malloc(A->col * sizeof(*A_n));
  for(i = 0; i < A->col; i++){
    A_n[i] = createVector(A->row);
    for(j = 0; j < A->row; j++){
      A_n[i]->data[j] = A->data[j][i];
    }
  }

  for(k = 0; k < A->col - 2; k++){
    W[k] = findW(A_n[k], k + 1);
    applyingLeftHouseHolder(W[k], A_n, A->col);
    applyingRightHouseHolder(W[k], A_n, A->col);

    /*
    applyingMatrixRightHouseHolder(W[k], P);
    */
    
    makeMatrixHouseHolder(W[k], Aux);
    Aux2 = P;
    P = multMatrix(P, Aux);
    free(Aux2);

    /***DEBUG***/
    if(DEBUG_HESS > 2){
      printf("MATRIX PARCIAL:\n");
      for(i = 0; i < A->col; i++){
        for(j = 0; j < A->col; j++){
          printf("%.2f ", A_n[j]->data[i]);
        }
        printf("\n");
      }
    }
    /**********/
  }
  /***DEBUG***/
  if(DEBUG_HESS > 2){
    for(j = 0; j < A->col - 2; j++){
      printf("W[%d]: ", j);
      for(i = 0; i < A->col; i++){
        printf("%f ", W[j]->data[i]);
      }
      printf("\n");
    }
  }
  /**********/


  H = createTridiagonal(A->row);
  for (i = 0; i < A->row - 1; i++){
    H->sup->data[i] = A_n[i + 1]->data[i];
    H->princ->data[i] = A_n[i]->data[i];
    H->sub->data[i] = A_n[i]->data[i + 1];
  }
  H->princ->data[A->row - 1] = A_n[A->row - 1]->data[A->row - 1];

  for(i = 0; i < A->col; i++){
    freeVector(A_n[i]);
  }
  free(A_n);

  /*só por agora*/
  for(i = 0; i < A->col - 2; i++){
    freeVector(W[i]);
  }
  free(W);


  return H;
}

/*****************************************************************************/
/* findW:                                                                    */
/* Devolve vetor w utilizado no processo de reflexão de Householder          */
/*  w' = x' - (||x'||/||y||)*y                                               */
/*  y  = e_{x->len}                                                          */
/*  lambda = ||x'|| / ||y||                                                  */
/*  considera-se x' como o subvetor de x das posições [ini] --> [x->len - 1] */
/*  w é o vetor de zeros nas posições [0] --> [ini] e vetor w'               */
/*  considera-se w' como o subvetor de w das posições [ini] --> [x->len - 1] */
/*****************************************************************************/
Vector* findW(Vector* x, int ini){
  int i;
  Vector *w;
  float lambda;

  w = createVector(x->len);
  for(i = ini; i < x->len; i++){
    w->data[i] = x->data[i];
  }

  lambda = dotProduct(w,w);
  lambda = (float)pow(lambda, 0.5);
  w->data[ini] = x->data[ini] - lambda;

  return w;
}

/*****************************************************************************/
/* createTridiagonal:                                                        */
/*****************************************************************************/
Tridiagonal* createTridiagonal(int len){
  Tridiagonal* aux = malloc(sizeof *aux);
  aux->len_princ = len;
  aux->princ = createVector(len);
  aux->sub = createVector(len - 1);
  aux->sup = createVector(len - 1);
  return aux;
}

/*****************************************************************************/
/* freeTridiagonal:                                                          */
/*****************************************************************************/
void freeTridiagonal(Tridiagonal* H){
  freeVector(H->princ);
  freeVector(H->sub);
  freeVector(H->sup);
  free(H);
}

/*****************************************************************************/
/* applyingMatrixRightHouseHolder:                                                  */
/* lambda => ||w||^2                                                         */
/* wt_a_2 => (w^t * a)*2                                                     */
/*****************************************************************************/
void applyingMatrixRightHouseHolder(Vector* w, Matrix* A){
  int j,i;
  Vector *aux;
  float lambda, wt_a_2, dot;

  lambda = dotProduct(w,w);
  if(lambda == 0) return;
  aux = createVector(A->col);
  for(i = 0; i < A->row; i++){
      cpyVectors(w, aux);
      dot = 0;
      for(j = 0; j < A->col; j++)
	dot+= A->data[i][j]*w->data[j];
      wt_a_2 = (dot * 2) / lambda;
      for(j = 0; j < A->col; j++)
	A->data[i][j] -= wt_a_2*A->data[i][j]; 
	  // subVectors(A_n[j], aux, A_n[j]);
  }
  free(aux);
}


/*****************************************************************************/
/* applyingLeftHouseHolder:                                                  */
/* lambda => ||w||^2                                                         */
/* wt_a_2 => (w^t * a)*2                                                     */
/*****************************************************************************/
void applyingLeftHouseHolder(Vector* w, Vector** A_n, int ncols){
  int j;
  Vector *aux;
  float lambda, wt_a_2;

  lambda = dotProduct(w,w);
  aux = createVector(A_n[0]->len);
  for(j = 0; j < ncols; j++){
    cpyVectors(w, aux);
    wt_a_2 = dotProduct(w, A_n[j]) * 2;
    if(lambda != 0.0) multByScalar((wt_a_2 / lambda), aux);
    subVectors(A_n[j], aux, A_n[j]);
  }
  free(aux);
}

/*****************************************************************************/
/* applyingRightHouseHolder:                                                 */
/* (A*H^t) = H*A^t                                                           */
/* lambda => ||w||^2                                                         */
/* wt_a_2 => (w^t * a)*2                                                     */
/*****************************************************************************/
void applyingRightHouseHolder(Vector* w, Vector** A_n, int nlins){
  int i, j;
  Vector *aux;
  float lambda, wt_a_2;

  lambda = dotProduct(w,w);
  aux = createVector(A_n[0]->len);
  
  for(i = 0; i < nlins; i++){
    cpyVectors(w, aux);
    /* wt_a_2 = dotProduct(w, A_n[i]) * 2;*/
    wt_a_2 = 0;
    for(j = 0; j < A_n[0]->len; j++){
      wt_a_2 += w->data[j]*A_n[j]->data[i];
    }
    wt_a_2 *= 2;
    /***/

    if(lambda != 0.0) multByScalar((wt_a_2 / lambda), aux);

    /*subVectors(A_n[i], aux, A_n[i]);*/
    for(j = 0; j < A_n[0]->len; j++){
      A_n[j]->data[i] -=  aux->data[j];
    }
    /***/
  }
  free(aux);
}

/*****************************************************************************/
/* makeMatrixHouseHolder:                                                    */
/*****************************************************************************/
void makeMatrixHouseHolder(Vector* w, Matrix* H){
  int i, j;
  float lambda;

  lambda = dotProduct(w, w);
  if(lambda != 0){
    lambda = 2 / lambda;
  }

  for(i = 0; i < w->len; i++){
    for(j = 0; j < w->len; j++){
      H->data[i][j] = -lambda * w->data[i] * w->data[j];
      if(i == j){
        H->data[i][j] += 1;
      }
    }
  }

}


Matrix* getP(){
  return P;
}

void freeP(){
  freeMatrix(P);
}
