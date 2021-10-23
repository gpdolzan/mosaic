// GRR20209948 Gabriel Pimentel Dolzan
#include "paramsIO.h"
#include "ppmManager.h"
#include "arrayManager.h"

int main(int argc, char* argv[])
{
	//Declare variables
	parameters params;
	tile input;
	tile output;
	tile* tilesArr;

	//Get parameters from argc/argv
	setIO(&params, argc, argv);

	scanDirectory(&params);

	//Allocate and fill tiles in tilesArray
	tilesArr = allocTiles(params.n_files);
	getTiles(&params, tilesArr);

	//Get input from STDIN
	getInput(&input);

	//Generate output based on tiles collected
	generateOutput(&input, tilesArr, &output, &params);

	//Print tile in STDOUT
	printTile(&output);

	//Check if memory was freed correctly
	if(freeParams(&params) && freeTiles(&input, &output, tilesArr, params.n_files))
		fprintf(stderr, "All memory allocations are freed\n");
	else
	{
		perror("Error while trying to free parameters/tiles array:");
		exit(1);
	}

	exit(0);
}