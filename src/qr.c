#include "qr.h"

void applyRightGivensRotation(Givens*, Matrix*);

Matrix *R;



void qrMethod(Vector* up,Vector* mid,Vector* low){

  Givens** vectorGivens;
  Vector *myUp, *myMid, *myLow;
  int n = mid->len;
  int k2, k, j, i, id;
  Matrix* mini = createMatrix(4,2);
  myUp = copyVector(up);
  myMid = copyVector(mid);
  myLow = copyVector(low);

  for(k2 = 0; k2 < 100; k2++){
    vectorGivens = qrDecomposition(myUp, myMid,myLow);
    
    /* Time to do R*Q. Apply all givens on the right side */
    for(i = 0; i < n-1; i++){
      id = vectorGivens[i]->i;
      /* populate mini */
      
      
      for(j = -2; j <= 1; j++) 
 	for(k = 0; k < 2; k++){
	  if(id + j >= 0 && id + j < n && k < n && k >= 0 )
	    mini->data[j + 2][k] = R->data[id + j][id + k];
	  else
	    mini->data[j + 2][k] = 0;
	}
      printf("Matrix R\n");
      printMatrix(R);

      
      printf("\n\nBefore rotation (id: %d): \n", id);

      printMatrix(mini);

      applyRightGivensRotation(vectorGivens[i], mini);

      printf("After rotation: \n");
      printMatrix(mini);

      /*Populate R*/
      for(j = -2; j <= 1; j++) 
 	for(k = 0; k < 2; k++){
	  if(id + j >= 0 && id + j < n && k < n && k >= 0 )
	    R->data[id + j][id + k] = mini->data[j + 2][k];
	}
      printf("\n");
    }

    /* Populate the vectors with the answers in R */
    for(i = 0; i < n-1; i++){
      myMid->data[i]= R->data[i][i];
      myLow->data[i]= R->data[i+1][i];
      myUp->data[i]= R->data[i][i+1];
    }
    myMid->data[n-1]= R->data[n-1][n-1];
      
    
  }

  printf("After some time frying, we got:\n");
  printMatrix(R);
  

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


/*
  Q = G[0]*G[1]*G[2]*(...)*G[n-1]

 */

Givens** qrDecomposition(Vector* up,Vector* mid,Vector* low){
  int m,n = mid->len;
  int i = 0,j,k;
  Matrix* mini = createMatrix(2,4);
  Givens** vectorGivens = malloc((n-1)*sizeof(Givens*));
  R = createMatrix(n,n);

  for(i = 0; i < n-1; i++){
    R->data[i][i] = mid->data[i];
    R->data[i+1][i+1] = mid->data[i+1];
    R->data[i+1][i] = low->data[i];
    R->data[i][i+1] = up->data[i];
  }

  
  for(i = 0; i < n-1; i++){
    //printMatrix(R);

    //printf("\n==========================\n");

    /* Populate mini */
    for(j = 0; j < 2; j++)
      for(k = -1; k < 3; k++){
	if(i + j >= 0 && i + j < n && i + k >= 0 && i + k < n)
	  mini->data[j][k + 1] = R->data[i + j][i + k];
	else
	  mini->data[j][k + 1] = 0;
      }

    vectorGivens[i] = givensRotation(mini->data[0][1],mini->data[1][1]);
    vectorGivens[i]->i = i;
    applyLeftGivensRotation(vectorGivens[i],mini);

    
    
    /*Populate R with the answer*/

    for(j = 0; j < 2; j++)
      for(k = -1; k < 3; k++){
	if(i + j >= 0 && i + j < n && i + k >= 0 && i + k < n)
	  R->data[i + j][i + k] = mini->data[j][k + 1];
      }

  }
  freeMatrix(mini);
  for(i = 0; i < n-1; i++){ /*Transpose all givens */
    vectorGivens[i]->sin = -1*vectorGivens[i]->sin;
  }
  //printMatrix(R);

  return vectorGivens;
}

float absF(float a){
  return a > 0? a : -1*a;
}

void applyLeftGivensRotation(Givens* gv, Matrix* m){
  float a = m->data[0][0];
  float b = m->data[0][1];
  float c = m->data[0][2];
  float d = m->data[0][3];
  float e = m->data[1][0]; 
  float f = m->data[1][1];  
  float g = m->data[1][2]; 
  float h = m->data[1][3];
  float sin = gv->sin;
  float cos = gv->cos;
  m->data[0][0] = cos*a - sin*e;
  m->data[1][0] = sin*a + cos*e;  
  m->data[0][1] = cos*b - sin*f;
  m->data[0][2] = cos*c - sin*g;
  m->data[1][1] = sin*b + cos*f;
  m->data[1][2] = cos*g + sin*c;
  m->data[0][3] = cos*d - sin*h;
  m->data[1][3] = d*sin + h*cos;
  
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
