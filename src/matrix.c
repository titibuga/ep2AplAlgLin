#include "matrix.h"

float** allocateMatrix(int row, int col){
  float **ret_val;
    int i;
 
    ret_val = (float **)calloc(row,sizeof(float *));
    if (ret_val == NULL) {
        perror("memory allocation failure");
        exit(EXIT_FAILURE);
    }
 
    for (i = 0; i < row; i++) {
      ret_val[i] = (float *)calloc(col,sizeof(float));
        if (ret_val[i] == NULL) {
            perror("memory allocation failure");
            exit(EXIT_FAILURE);
        }
    }
 
    return ret_val;
}

Matrix* createMatrix(int row, int col){
  Matrix* aux = malloc(sizeof *aux);
  int i,j;
  aux->row = row;
  aux->col = col;
  aux->data = allocateMatrix(row,col);
  
  return aux; 
}

Matrix* createTranspose(Matrix* m){
  Matrix* mt = createMatrix(m->col, m->row);
  int i = 0, j = 0;

  for(i = 0; i < m->row; i++)
    for(j = 0; j < m->col; j++)
      mt->data[j][i] = m->data[i][j];

  return mt;
}

void freeMatrix(Matrix* mat){
  deallocateMatrix(mat->data,mat->row);
  free(mat);
}

void populateMatrix(Matrix* mat,int** matrix){
  int i,j;
  
  for(i = 0; i < mat->row; i++)
    for(j = 0; j < mat->col; j++)
      mat->data[i][j] = (float)matrix[i][j];
}

void printMatrix(Matrix* mat){
  int i,j;
  for(i = 0; i < mat->row; i++){
    for(j = 0; j < mat->col; j++){
      printf("%f ",mat->data[i][j]);
    }
    printf("\n");
  }
}

Matrix* multMatrix(Matrix* A, Matrix* B){
  Matrix* C = createMatrix(A->row,B->col);
  int i,j,k;

  for(i = 0; i < A->row; i++)
    for(k = 0; k < B->row; k++)
      for(j = 0; j < B->col; j++)
	C->data[i][j] += A->data[i][k]*B->data[k][j];
  
  return C;
}

void deallocateMatrix(float **matrix, int row){
    int i;
    
    for (i = 0; i < row; i++) free(matrix[i]);
    free(matrix);
}

