#include "qr.h"

#define EPS 0.001

void applyRightGivensRotation(Givens*, Matrix*);
Matrix* createQ(Givens** ,int);
Matrix* createGivensMatrix(Givens*,int);

Matrix *R,*Q;


void qrMethod(Vector* up,Vector* mid,Vector* low){

  Givens** vectorGivens;
  Vector *myUp, *myMid, *myLow;
  int n = mid->len;
  int notConverged = 1;
  int k2, k, j, i, id;
  Matrix *aux,*aux2;
  Matrix* mini = createMatrix(4,2);
  Matrix* q = createMatrix(mid->len,mid->len);
  myUp = copyVector(up);
  myMid = copyVector(mid);
  myLow = copyVector(low);

  for(i = 0; i < mid->len; i++)
    for(j = 0; j < mid->len; j++)
      q->data[i][j] = (float)(i==j);
  
  R = NULL;
  for(k2 = 0; notConverged; k2++){
    notConverged = 0;
    //if(k2 >= 10) exit(0);
    if(R != NULL) freeMatrix(R);
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

      applyRightGivensRotation(vectorGivens[i], mini);

      /*Populate R*/
      for(j = -2; j <= 1; j++) 
 	for(k = 0; k < 2; k++){
	  if(id + j >= 0 && id + j < n && k < n && k >= 0 ){
	    R->data[id + j][id + k] = mini->data[j + 2][k];
	    if(id + j != id + k && R->data[id + j][id + k] > EPS)
	      notConverged = 1;
	  }
	}

      /* printf("\n"); */
      /* printMatrix(R); */
      /* printf("\n"); */
    }

    /* Populate the vectors with the answers in R */
    for(i = 0; i < n-1; i++){
      myMid->data[i]= R->data[i][i];
      /* Force it to be symmetric */
       R->data[i][i+1] = myUp->data[i] = myLow->data[i]= R->data[i+1][i];
    }
    myMid->data[n-1]= R->data[n-1][n-1];
    //aux = q;
    //aux2 = createQ(vectorGivens,mid->len);
    //q = multMatrix(q,aux2);
    
    
    for(i = 0; i < n-1;i++){
      applyRightFullGivens(vectorGivens[i], q);
      free(vectorGivens[i]);
    }
    //freeMatrix(aux);
    //freeMatrix(aux2);
    free(vectorGivens);
  }

  Q = q;
  freeMatrix(mini);
  freeVector(myUp);
  freeVector(myMid);
  freeVector(myLow);
}

void applyRightFullGivens(Givens* gv, Matrix* A){
  int i, id = gv->i;
  float aid, aid1;
  float s = gv->sin, c = gv->cos;
  for(i = 0; i < A->row; i++){
    aid = A->data[i][id];
    aid1 = A->data[i][id+1];
    A->data[i][id] = aid*c + aid1*s;
    A->data[i][id+1] = -aid*s + aid1*c;
  }
}

Matrix* createQ(Givens** givens,int n){
  Matrix* Q = createGivensMatrix(givens[0],n);
  int i;
  
  for(i = 1; i < n-1; i++){
    Matrix* A = createGivensMatrix(givens[i],n);
    Matrix* aux =  Q;
    Q = multMatrix(Q,A);
    freeMatrix(A);
    freeMatrix(aux);
  }

  return Q;
}

Matrix* createGivensMatrix(Givens* g,int n){
  Matrix* G = createMatrix(n,n);
  int i,j;

  for(i = 0; i < n; i++)
    for(j = 0; j < n; j++)
      G->data[i][j] = (float)( i == j );
    
  G->data[g->i][g->i] = g->cos;
  G->data[g->i][g->i+1] = -1*g->sin;
  G->data[g->i+1][g->i] = g->sin;
  G->data[g->i+1][g->i+1] = g->cos;

  return G;
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
  Q = G[0]*G[1]*G[2]*(...)*G[n-2]

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

Matrix* getR(){
  return R;
}

Matrix* getQ(){
  return Q;
}
