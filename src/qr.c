#include "qr.h"

Matrix *R;


void qrMethod(Vector* up,Vector* mid,Vector* low){

  Givens** vectorGivens;
  Vector *myUp, *myMid, *myLow;
  int n = mid->len;
  int k, i, id;
  myUp = copyVector(up);
  myMid = copyVector(mid);
  myLow = copyVector(low);

  for(k = 0; k < 100; k++){
    vectorGivens = qrDecomposition(myUp, myMid,myLow);
    
    /* Time to do R*Q. Apply all givens on the right side */
    for(i = n-1; i >= 0; i--){
      id = vectorGivens[i]->i;
      
    }
  }

  

}

void applyRightGivensRotation(Givens* gv, Matrix* m){
  int i,j;
  Matrix* mt = createTranspose(m);
  gv->sin = -1*gv->sin; /* transposing givens */
  applyLeftGivensRotation(gv, mt);
  gv->sin = -1*gv->sin; /* transposing givens (again)*/
  
  for(i = 0; i < m->row; i++)
    for(j = 0; j < m->col; j++)
      m->data[i][j] = mt->data[j][i];

  freeMatrix(mt);
  
}


Givens** qrDecomposition(Vector* up,Vector* mid,Vector* low){
  int m,n = mid->len;
  int i = 0;
  Matrix* mini = createMatrix(2,3);
  Givens** vectorGivens = malloc(n*sizeof(Givens*));
  R = createMatrix(n,n);

  for(i = 0; i < n-1; i++){
    R->data[i][i] = mid->data[i];
    R->data[i+1][i+1] = mid->data[i+1];
    R->data[i+1][i] = low->data[i];
    R->data[i][i+1] = up->data[i];
  }

  
  for(i = 0; i < n-1; i++){
    printMatrix(R);

    printf("\n==========================\n");

    mini->data[0][0] = R->data[i][i];
    mini->data[0][1] = R->data[i][i+1];
    mini->data[1][0] = R->data[i+1][i];
    mini->data[1][1] = R->data[i+1][i+1];
    if(i < n-2){
      mini->data[0][2] = R->data[i][i+2];
      mini->data[1][2] = R->data[i+1][i+2];
    }
    vectorGivens[i] = givensRotation(mini->data[0][0],mini->data[1][0]);
    applyLeftGivensRotation(vectorGivens[i],mini);
    R->data[i][i] = mini->data[0][0];
    R->data[i][i+1] = mini->data[0][1];
    R->data[i+1][i] = mini->data[1][0]; /*Se pa por arredondamento não
					  dê 0 aqui, mas é pra dar */
    R->data[i+1][i+1] = mini->data[1][1]; 
    if(i < n-2){
      R->data[i][i+2] = mini->data[0][2];
      R->data[i+1][i+2] = mini->data[1][2];
    }
  }
  freeMatrix(mini);
  for(i = 0; i < n; i++){ /*Transpose all givens */
    vectorGivens[i]->sin = -1*vectorGivens[i]->sin;
  }
  printMatrix(R);
}

float absF(float a){
  return a > 0? a : -1*a;
}

void applyLeftGivensRotation(Givens* g, Matrix* m){
  float a = m->data[0][0];
  float b = m->data[0][1];
  float c = m->data[1][0];
  float d = m->data[1][1];
  float e = m->data[0][2]; 
  float f = m->data[1][2];
  float sin = g->sin;
  float cos = g->cos;
  m->data[0][0] = cos*a - sin*c;
  m->data[0][1] = cos*b - sin*d;
  m->data[1][0] = sin*a + cos*c;
  m->data[1][1] = cos*d + sin*b;
  m->data[0][2] = cos*e - sin*f;
  m->data[1][2] = e*sin + f*cos;
  
}

Givens* createGivens(float s,float c){ 
  Givens* aux = malloc(sizeof *aux);
  aux->sin = s;
  aux->cos = c;
  aux->i = 0;
  printf("Seno: %f Cosseno: %f\n",s,c);
  return aux;
}

Givens* givensRotation(float a,float b){ /*pra zerar b, gg */
  float c,s,r,t,u;
  if(b == 0.0){
    c = copysign(1,a);
    s = 0;
  }else if(a == 0.0){
    c = 0;
    s = -1*copysign(1,b);
  }else{
    if(absF(b) > absF(a)){
      t = a / b;
      u = copysign(sqrt(1+t*t),b);
      s = -1 / u;
      c = -1*s*t;
    }else{
      t = b / a;
      u = copysign(sqrt(1+t*t),a);
      c = 1 / u;
      s = -1*c*t;
    }
  }
  return createGivens(s,c);
}



/* #if 0 */

/* function [Q,R] = qrgivens(A) */
/*   [m,n] = size(A); */
/*   Q = eye(m); */
/*   R = A; */

/*   for j = 1:n */
/*     for i = m:-1:(j+1) */
/*       G = eye(m); */
/*       [c,s] = givensrotation( R(i-1,j),R(i,j) ); */
/*       G([i-1, i],[i-1, i]) = [c -s; s c;] */
/*       R = G'*R; */
/*       Q = Q*G; */
/*     end */
/*   end */

/* end */
