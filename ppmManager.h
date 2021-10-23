// GRR20209948 Gabriel Pimentel Dolzan
#ifndef PPMMANAGER_H
#define PPMMANAGER_H

//Included libraries
#include <math.h>
#include "paramsIO.h"

//Constants
#define FILETYPE_SIZE 3

//RGB structure
typedef struct
{
	int red;
	int green;
	int blue;
}RGB;

//Tile structure
typedef struct
{
	char fileType[3];
	int width;
	int height;
	int highestColor;
	RGB avgTileValue;
	int flagDarkMode;
	int** arr;
}tile;

//Functions Headers
tile* allocTiles(int number_of_files);
void initializeTile(tile* tile);
void fillTile(tile* tile, char* fileAddress);
void getAvgTileValue(tile* tile);
void getTiles(parameters* params, tile* tilesArr);
tile getNextTile(char* fileAddress);
void getInput(tile* tile);
void generateOutput(tile* input, tile* tilesArr, tile* output, parameters* params);
void getAvgTileValueInput(int x, int y, int xTarget, int yTarget, tile* tile, RGB* avgTileInput);
int getTileID(tile* tilesArr, RGB avgTileInput, int number_of_files);
void placeTileOnOutput(int x, int y, int xTarget, int yTarget, int tileID, tile* tilesArr, tile* output, int cont);
long int calculateColorScore(RGB color1, RGB color2);
int freeTiles(tile* input, tile* output, tile* tilesArr, int number_of_files);
void printTile(tile* tile);

#endif