// GRR20209948 Gabriel Pimentel Dolzan
#include "arrayManager.h"

//Allocate array based on width and height acquired
int** arrAlloc(int width, int height)
{
	int** arr;
	arr = malloc(height * sizeof(int*));
	if(arr == NULL)
	{
		perror("Memoria insuficiente");
		exit(1);
	}
	for(int i = 0; i < height; i++)
	{
		arr[i] = malloc(width * sizeof(int));
		if(arr[i] == NULL)
		{
			perror("Memoria insuficiente");
			exit(1);
		}
	}
	return arr;
}

//Initialize array with 0s based on width and height
void initializeArr(int** arr, int width, int height)
{
	for(int i = 0; i < height; i++)
		for(int j = 0; j < width; j++)
			arr[i][j] = 0;
}

//Free array - avoiding memory leak
int freeArr(int** arr, int height)
{
	for(int i = 0; i < height; i++)
		free(arr[i]);
	free(arr);
	return 1;
}