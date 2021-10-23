// GRR20209948 Gabriel Pimentel Dolzan
#ifndef PARAMSIO_H
#define PARAMSIO_H

//Included Libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

//Parameters structure
typedef struct
{
	char* input_name;
	char* output_name;
	char* directory_name;
	int n_files;
}parameters;

//Functions headers
void setIO(parameters* params, int argc, char* argv[]);
int readParams(parameters* params, int argc, char* argv[]);
int verifyDirectory(parameters* params);
void scanDirectory(parameters* params);
int freeParams(parameters* params);

#endif