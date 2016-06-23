#ifndef READER_H
#define READER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct pgmData {
    int row;
    int col;
    int max_gray;
    int **matrix;
} PGMData;

int** allocateDynamicMatrix(int,int);
void deallocateDynamicMatrix(int**,int);
void skipComments(FILE*);
PGMData* readPGM(const char*,PGMData*);
void writePGM(const char*,const PGMData*);

#endif
