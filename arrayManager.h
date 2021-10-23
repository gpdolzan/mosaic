// GRR20209948 Gabriel Pimentel Dolzan
#ifndef ARRAYMANAGER_H
#define ARRAYMANAGER_H

//Included libraries
#include <stdlib.h>
#include <stdio.h>

//Functions headers
int** arrAlloc(int width, int height);
void initializeArr(int** arr, int width, int height);
int freeArr(int** arr, int height);

#endif