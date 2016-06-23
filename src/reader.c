#include "reader.h"

int** allocateDynamicMatrix(int row, int col){
    int **ret_val;
    int i;
 
    ret_val = (int **)malloc(sizeof(int *) * row);
    if (ret_val == NULL) {
        perror("memory allocation failure");
        exit(EXIT_FAILURE);
    }
 
    for (i = 0; i < row; i++) {
        ret_val[i] = (int *)malloc(sizeof(int) * col);
        if (ret_val[i] == NULL) {
            perror("memory allocation failure");
            exit(EXIT_FAILURE);
        }
    }
 
    return ret_val;
}
 
void deallocateDynamicMatrix(int **matrix, int row){
    int i;
    for (i = 0; i < row; i++) free(matrix[i]);
    free(matrix);
}


void skipComments(FILE *fp){
    int ch;
    char line[100];
 
    while ((ch = fgetc(fp)) != EOF && isspace(ch));
    if (ch == '#') {
        fgets(line, sizeof(line), fp);
        skipComments(fp);
    } else fseek(fp, -1, SEEK_CUR);
}


PGMData* readPGM(const char *file_name, PGMData *data){
    FILE *pgmFile;
    char version[3];
    int i, j;
    
    pgmFile = fopen(file_name, "rb");
    if (pgmFile == NULL) {
        perror("cannot open file to read");
        exit(EXIT_FAILURE);
    }
 
    fgets(version, sizeof(version), pgmFile);
    if (strcmp(version, "P2")) {
        fprintf(stderr, "Wrong file type!\n");
        exit(EXIT_FAILURE);
    }
 
    skipComments(pgmFile);
    fscanf(pgmFile, "%d", &data->col);
    skipComments(pgmFile);
    fscanf(pgmFile, "%d", &data->row);
    skipComments(pgmFile);
    fscanf(pgmFile, "%d", &data->max_gray);
    fgetc(pgmFile);
 
    data->matrix = allocateDynamicMatrix(data->row, data->col);
    for (i = 0; i < data->row; i++)
      for (j = 0; j < data->col; j++)
	fscanf(pgmFile,"%d",&data->matrix[i][j]);

    fclose(pgmFile);
    return data;
 
}
 
 
void writePGM(const char *filename, const PGMData *data){
    FILE *pgmFile;
    int i, j;
 
    pgmFile = fopen(filename, "wb");
    if (pgmFile == NULL) {
        perror("cannot open file to write");
        exit(EXIT_FAILURE);
    }
 
    fprintf(pgmFile, "P2 \n");
    fprintf(pgmFile, "%d %d \n", data->col, data->row);
    fprintf(pgmFile, "%d \n", data->max_gray);
    
    for (i = 0; i < data->row; i++){
      for (j = 0; j < data->col; j++){ 
	fprintf(pgmFile,"%d ",data->matrix[i][j]);
      }
      fprintf(pgmFile,"\n");
    }
	  
      
    fclose(pgmFile);
    deallocateDynamicMatrix(data->matrix, data->row);
}

