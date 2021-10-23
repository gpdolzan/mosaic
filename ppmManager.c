// GRR20209948 Gabriel Pimentel Dolzan
#include "ppmManager.h"
#include "arrayManager.h"
#include "paramsIO.h"

//Allocate tiles based on number of tiles in a certain directory
tile* allocTiles(int number_of_files)
{
	tile* tilesArr;

	tilesArr = malloc(number_of_files * sizeof(tile));
	if(tilesArr == NULL)
	{
		perror("Insufficient memory");
		exit(1);
	}
	return tilesArr;
}

//Initialize tiles structure with 0s
void initializeTile(tile* tile)
{
	int strSize = FILETYPE_SIZE;
	for(int i = 0; i < strSize; i++)
		tile->fileType[i] = 0;
	tile->width = 0;
	tile->height = 0;
	tile->highestColor = 0;
	tile->avgTileValue.red = 0;
	tile->avgTileValue.green = 0;
	tile->avgTileValue.blue = 0;
}

//Calculate average RGB tile value
void getAvgTileValue(tile* tile)
{
	long int sumRed = 0;
	long int sumGreen = 0;
	long int sumBlue = 0;
	int colorTypeFlag = 0;
	long int arrSize = tile->width * tile->height;

	//Iterate through the 2d array adding to each sum
	for(int i = 0; i < tile->height; i++)
	{
		for(int j = 0; j < tile->width; j++)
		{
			if(colorTypeFlag == 0)
			{
				sumRed += tile->arr[i][j];
				colorTypeFlag++;
			}
			else if(colorTypeFlag == 1)
			{
				sumGreen += tile->arr[i][j];
				colorTypeFlag++;
			}
			else if(colorTypeFlag == 2)
			{
				sumBlue += tile->arr[i][j];
				colorTypeFlag = 0;
			}
		}
	}

	//Then I get the average by dividing each sum by 1/3 of the array size
	tile->avgTileValue.red = sumRed/(arrSize/3);
	tile->avgTileValue.green = sumGreen/(arrSize/3);
	tile->avgTileValue.blue = sumBlue/(arrSize/3);
}

//Get tiles by searching the directory then place them on tiles array
void getTiles(parameters* params, tile* tilesArr)
{
	DIR* dirstream;
	struct dirent* direntry;
	int k = 0;
	int strSize = 0;
	char* fileAddress;

	fprintf(stderr, "Storing tiles and calculating average colors\n");

	//try to open directory and verify if it opened correctly
	dirstream = opendir(params->directory_name);
	if(!dirstream)
	{
		perror("Could not open directory");
	}
	else if(dirstream)
	{
		//While there are things to verify in the directory
		while((direntry = readdir(dirstream)) != NULL)
		{
			//Verify if direntry contains a file
			if(direntry->d_type == DT_REG)
			{
				strSize = (strlen(params->directory_name) + strlen(direntry->d_name) + 2);
				fileAddress = malloc(strSize * sizeof(char));
				if(fileAddress == NULL)
				{
					perror("Insufficient memory");
					exit(1);
				}
				//creates a string that contains the path to a certain .ppm tile image
				sprintf(fileAddress, "%s/%s", params->directory_name, direntry->d_name);
				//Places tile on array
				tilesArr[k] = getNextTile(fileAddress);
				//Free file address to avoid memory leak
				free(fileAddress);
				k++;
			}
		}
		//Close directory
		closedir(dirstream);
		fprintf(stderr, "%d tiles processed\n", params->n_files);
		fprintf(stderr, "Tile size is %dx%d\n", tilesArr[0].width/3, tilesArr[0].height);
	}
}

//Create and prepare tile for insertion in tiles array
tile getNextTile(char* fileAddress)
{
	tile tile;
	initializeTile(&tile);
	fillTile(&tile, fileAddress);
	return tile;
}

//Get input image information
void getInput(tile* tile)
{
	fprintf(stderr, "Reading input image\n");

	int num_value;
	char character;
	unsigned char byte_value;

	initializeTile(tile);

	//Get file Type
	fscanf(stdin, "%s\n", tile->fileType);

	//Comment Skip
	fscanf(stdin, "%c", &character);
	if(character == '#')
    {
        while((character = fgetc(stdin)) != EOF)
        {
            if(character == '\n') 
            {
                if ((character = fgetc(stdin)) != '#')
                    break;
            }
        }
    }
    fseek(stdin, -sizeof(character), SEEK_CUR);

    //Get Width, Height and HighestColor
  	fscanf(stdin, "%d", &(tile->width));
  	tile->width *= 3;
  	fscanf(stdin, "%d", &(tile->height));
  	fscanf(stdin, "%d", &(tile->highestColor));

  	//Allocate array
  	tile->arr = arrAlloc(tile->width, tile->height);
  	initializeArr(tile->arr, tile->width, tile->height);

  	//Verify type then fill array
  	if(tile->fileType[1] == '3') //PPM P3
  	{
  		for(int i = 0; i < tile->height; i++)
  		{
  			for(int j = 0; j < tile->width; j++)
  			{
  				fscanf(stdin, "%d", &num_value);
  				tile->arr[i][j] = num_value;
  			}
  		}
  	}
  	else //PPM P6
  	{
  		fread(&byte_value, sizeof(char), 1, stdin);
		for(int i = 0; i < tile->height; i++)
		{
			for(int j = 0; j < tile->width; j++)
			{
				fread(&byte_value, sizeof(char), 1, stdin);
				tile->arr[i][j] = byte_value;
			}
		}
  	}
  	fprintf(stderr, "Input image is PPM %s, %dx%d pixels\n", tile->fileType, tile->width/3, tile->height);
}

void fillTile(tile* tile, char* fileAddress)
{
	int num_value;
	char character;
	unsigned char byte_value;
	FILE* tileFile;

	tileFile = fopen(fileAddress, "r");
	if(!tileFile)
	{
		perror("Error while trying to open file");
		exit(1);
	}

	//File Type
	fscanf(tileFile, "%s", tile->fileType);

	//Comment Skip
	fscanf(tileFile, "%c", &character);
	if(character == '#')
    {
        while((character = fgetc(tileFile)) != EOF)
        {
            if(character == '\n') 
            {
                if ((character = fgetc(tileFile)) != '#')
                    break;
            }
        }
    }
    fseek(tileFile, -sizeof(character), SEEK_CUR);

  	//Get Width, Height & HighestColor
  	fscanf(tileFile, "%d", &(tile->width));
  	tile->width *= 3;
  	fscanf(tileFile, "%d", &(tile->height));
  	fscanf(tileFile, "%d", &(tile->highestColor));

  	//Allocate array
  	tile->arr = arrAlloc(tile->width, tile->height);
  	initializeArr(tile->arr, tile->width, tile->height);

  	//Verify type then fill array
  	if(tile->fileType[1] == '3') //PPM P3
  	{
  		for(int i = 0; i < tile->height; i++)
  		{
  			for(int j = 0; j < tile->width; j++)
  			{
  				fscanf(tileFile, "%d", &num_value);
  				tile->arr[i][j] = num_value;
  			}
  		}
  	}
  	else //PPM P6
  	{
  		fread(&byte_value, sizeof(char), 1, tileFile);
		for(int i = 0; i < tile->height; i++)
		{
			for(int j = 0; j < tile->width; j++)
			{
				fread(&byte_value, sizeof(char), 1, tileFile);
				tile->arr[i][j] = byte_value;
			}
		}
  	}
  	fclose(tileFile);

  	//Get average RGB color value
  	getAvgTileValue(tile);
}

//Print output tile in file(STDOUT)
void printTile(tile* tile)
{
	fprintf(stderr, "Writing output file\n");

	int num;
	unsigned char byte_value;

	printf("%s\n", tile->fileType);
	printf("%d %d\n", tile->width/3, tile->height);
	printf("%d\n", tile->highestColor);

	if(tile->fileType[1] == '3')
	{
		for(int i = 0; i < tile->height; i++)
		{
			for(int j = 0; j < tile->width; j++)
			{
				num = tile->arr[i][j];
				fprintf(stdout, "%d ", num);
			}
			printf("\n");
		}
	}
	else
	{
		for(int i = 0; i < tile->height; i++)
		{
			for(int j = 0; j < tile->width; j++)
			{
				byte_value = tile->arr[i][j];
				fwrite(&byte_value, sizeof(char), 1, stdout);
			}
		}
	}
}

//Generate output file using input image and tiles array
void generateOutput(tile* input, tile* tilesArr, tile* output, parameters* params)
{
	fprintf(stderr, "Building mosaic image\n");

	int tileID = -1;
	int x = 0;
	int y = 0;
	int xTarget = tilesArr[0].width;
	int yTarget = tilesArr[0].height;
	int xTotal = input->width;
	int yTotal = input->height;

	strncpy(output->fileType, input->fileType, 3);
	output->width = input->width;
	output->height = input->height;
	output->highestColor = input->highestColor;


	output->arr = arrAlloc(xTotal, yTotal);
	initializeArr(output->arr, xTotal, yTotal);

	RGB avgTileInput;

	int cont = 0;

	/*
		Double while loop checks if x or y position is higher than the original image, if it is I crop it to make resolution of the output
		image identical as the input image. Also I calculate average RGB values based on the x, y coordinates to xTarget, yTarget coordinates
		on the input image, then I check to see which tile fits the best within that threshold that I just calculated, then I store that tile 
		position using "tileID". Finally, using x,y and xTarget,yTarget I can place on the input image exactly where I left off on the last loop
		if it is the first loop, it will just place starting on 0,0 to xTarget,yTarget.
	*/
	while(y < yTotal)
	{
		if((y + yTarget) > yTotal)
			yTarget = yTotal - y;

		while(x < xTotal)
		{
			if((x + xTarget) > xTotal)
				xTarget = xTotal - x;

			getAvgTileValueInput(x, y, xTarget, yTarget, input, &avgTileInput);
			tileID = getTileID(tilesArr, avgTileInput, params->n_files);
			placeTileOnOutput(x, y, xTarget, yTarget, tileID, tilesArr, output, cont);
			cont++;
			x = x + xTarget;
		}
		x = 0;
		xTarget = tilesArr[0].width;
		y = y + yTarget;
	}
}

//Calculate average rgb color on a specific tile
void getAvgTileValueInput(int x, int y, int xTarget, int yTarget, tile* tile, RGB* avgTileInput)
{
	avgTileInput->red = 0;
	avgTileInput->green = 0;
	avgTileInput->blue = 0;

	long int sumRed = 0;
	long int sumGreen = 0;
	long int sumBlue = 0;
	long int arrSize = xTarget * yTarget;
	int colorTypeFlag = 0;
	int xPos = x + xTarget;
	int yPos = y + yTarget;

	//Iterate through the 2d array and sum each RGB color
	for(int i = y; i < yPos; i++)
	{
		for(int j = x; j < xPos; j++)
		{
			if(colorTypeFlag == 0)
			{
				sumRed += tile->arr[i][j];
				colorTypeFlag++;
			}
			else if(colorTypeFlag == 1)
			{
				sumGreen += tile->arr[i][j];
				colorTypeFlag++;
			}
			else if(colorTypeFlag == 2)
			{
				sumBlue += tile->arr[i][j];
				colorTypeFlag = 0;
			}
		}
	}

	//Then divide it by 1/3 of the total array size
	avgTileInput->red = sumRed/(arrSize/3);
	avgTileInput->green = sumGreen/(arrSize/3);
	avgTileInput->blue = sumBlue/(arrSize/3);
}

//Place tile on output image based on where x and y coordinates are and where they will be
void placeTileOnOutput(int x, int y, int xTarget, int yTarget, int tileID, tile* tilesArr, tile* output, int cont)
{
	int xPos = x + xTarget;
	int yPos = y + yTarget;

	int xTileCounter = 0;
	int yTileCounter = 0;

	//Placement of tile in a certain x,y coordinate to xPos,yPos coordinate in output array
	for(int i = y; i < yPos; i++)
	{
		for(int j = x; j < xPos; j++)
		{
			output->arr[i][j] = tilesArr[tileID].arr[yTileCounter][xTileCounter];
			xTileCounter++;
		}
		yTileCounter++;
		xTileCounter = 0;
	}
}

//Get tile Identification based on score calculate
int getTileID(tile* tilesArr, RGB avgTileInput, int number_of_files)
{
	int tileID = 0;
	long int score = 0;
	long int newScore = 0;

	//Iterate through tile array calculating score of each tile based on the average RGB of a subarray in the input image
	for(int i = 0; i < number_of_files; i++)
	{
		newScore = calculateColorScore(avgTileInput, tilesArr[i].avgTileValue);
		if(newScore > score)
		{
			score = newScore;
			tileID = i;
		}
	}
	//Return identification
	return tileID;
}

//Calculate color score based on color1 = input subarray RGB average and color2 = tiles RGB average
long int calculateColorScore(RGB color1, RGB color2)
{
	long int calcRed;
	long int calcGreen;
	long int calcBlue;
	long int sum;
	int colorsSum = 0;

	colorsSum = color1.red + color1.green + color1.blue;

	/*
		The usage of this 2 formulas creates more contrast on the produced image, so it makes it more
		vivid/alive than the usage of only one of them. The first formula increases the chance that a
		brigther color will be selected, while the second formula increases the chance that a darker color
		will be selected instead.
	*/

	//Uses first formula
	if((colorsSum > 382) || (color1.red > 170) || (color1.green > 170) || (color1.blue > 170))
	{
		//This formula makes images with brighter averages have a higher chance at being selected
		calcRed = (color1.red) * abs(255 - abs(color1.red - color2.red));
		calcGreen = (color1.green) * abs(255 - abs(color1.green - color2.green));
		calcBlue = (color1.blue) * abs(255 - abs(color1.blue - color2.blue));
	}
	else //Uses second formula
	{
		//This formula makes images with darker averages have a higher chance at being selected
		calcRed = (255 - color1.red) * abs(255 - abs(color1.red - color2.red));
		calcGreen = (255 - color1.green) * abs(255 - abs(color1.green - color2.green));
		calcBlue = (255 - color1.blue) * abs(255 - abs(color1.blue - color2.blue));
	}

	sum = calcRed + calcGreen + calcBlue;
	return sum;
}

//Free tiles array and all image arrays - avoiding memory leak
int freeTiles(tile* input, tile* output, tile* tilesArr, int number_of_files)
{
	freeArr(input->arr, input->height);
	freeArr(output->arr, output->height);
	for(int i = 0; i < number_of_files; i++)
		freeArr(tilesArr[i].arr, tilesArr[i].height);
	free(tilesArr);
	return 1;
}