// GRR20209948 Gabriel Pimentel Dolzan
#include "paramsIO.h"

//Function to arrange I/O and Directory folder accordingly
void setIO(parameters* params, int argc, char* argv[])
{
	//Initialize pointers with null char
	params->input_name = malloc(1 * sizeof(char));
	params->input_name[0] = 0;
	params->output_name = malloc(1 * sizeof(char));
	params->output_name[0] = 0;
	params->directory_name = malloc(1 * sizeof(char));
	params->directory_name[0] = 0;

	//Read parameters
	if(readParams(params, argc, argv))
	{
		//Check if directory path exists
		if(verifyDirectory(params))
			fprintf(stderr, "Reading tiles from %s\n", params->directory_name);
		else
		{
			fprintf(stderr, "Error while verifying directory parameters!\n");
			exit(1);
		}
	}
	else
	{
		fprintf(stderr, "Error while reading input parameters\n");
		exit(1);
	}

	//Changes I/O if necessary
	if(params->input_name[0] != 0)
		freopen(params->input_name, "r", stdin);

	if(params->output_name[0] != 0)
		freopen(params->output_name, "w", stdout);
}

//Read parameters sent via terminal(argc/argv)
int readParams(parameters* params, int argc, char* argv[])
{
	int strSize = 0;

	//Iterate through argv using argc as stopping point
	for(int i = 1; i < argc; i++)
	{
		//Receive -i as parameter (INPUT/STDIN)
		if(strncmp(argv[i], "-i", 2) == 0)
		{
			if(argv[i+1] == NULL)
			{
				fprintf(stderr, "Parameter -i caled, but informations is missing, aborting execution\n");
				exit(1);
			}
			else
			{
				free(params->input_name);
				strSize = (strlen(argv[i+1]) + 1);
				params->input_name = malloc(strSize * sizeof(char));
				if(params->input_name == NULL)
				{
					perror("Insufficient memory");
					exit(1);
				}
				strncpy(params->input_name, argv[i+1], strSize);
			}
		}
		//Receive -o as parameter (OUTPUT/STDOUT)
		if(strncmp(argv[i], "-o", 2) == 0)
		{
			if(argv[i+1] == NULL)
			{
				fprintf(stderr, "Parameter -o caled, but informations is missing, aborting execution...\n");
				exit(1);
			}
			else
			{
				free(params->output_name);
				strSize = (strlen(argv[i+1]) + 1);
				params->output_name = malloc(strSize * sizeof(char));
				if(params->output_name == NULL)
				{
					perror("Insufficient memory");
					exit(1);
				}
				strncpy(params->output_name, argv[i+1], strSize);
			}
		}
		//Receive -p as parameter (DIRECTORY/PATH of tiles)
		if(strncmp(argv[i], "-p", 2) == 0)
		{
			if(argv[i+1] == NULL)
			{
				fprintf(stderr, "Parameter -p caled, but informations is missing, aborting execution...\n");
				exit(1);
			}
			else
			{
				free(params->directory_name);
				strSize = (strlen(argv[i+1]) + 1);
				params->directory_name = malloc(strSize * sizeof(char));
				if(params->directory_name == NULL)
				{
					perror("Insufficient memory");
					exit(1);
				}
				strncpy(params->directory_name, argv[i+1], strSize);
			}
		}
		if(strncmp(argv[i], "-h", 2) == 0)
		{
			fprintf(stderr, "Mosaic program made for Programacao 2!\n");
			fprintf(stderr, "To use this program, you need to pass a few arguments before execution!\n");
			fprintf(stderr, "-i or <: sends main image to the program\n  this image will be used to create the mosaic\n"); 
			fprintf(stderr, "  Remember you need to send the name of the file right after the command -i or <\n  if you dont, the program won't work correctly.\n");
			fprintf(stderr, "-o or >: sets the name of the output image to the program\n  this will be where the mosaic will be sent\n");
			fprintf(stderr, "  Remember you need to send the name of the file right after the command -o or >\n  if you dont, the program won't work correctly.\n");
			fprintf(stderr, "-p: sends to the program the path directory containing all the tiles\n  this will be used to create the mosaic\n");
			fprintf(stderr, "  Remember you need to send the path of the directory containing said tiles\n  the program will assume its default directory: ./tiles.\n");
			fprintf(stderr, "If all the parameters are passed in correctly, a mosaic will be created\n");
			fprintf(stderr, "Please run ./mosaico with all the parameters correctly\nDo not use -h, the program wont run to completion\n");
			fprintf(stderr, "It will only display this help message then abort execution.\n");
			fprintf(stderr, "Thanks for using mosaic by Gabriel Pimentel Dolzan!\n");
			exit(1);
		}
	}
	return 1;
}

//Verify if directory needs to be set as default
int verifyDirectory(parameters* params)
{
	char* defaultDir = "./tiles";
	int strSize = 0;

	//Directory is not default
	if(params->directory_name[0] == 0)
	{
		free(params->directory_name);
		strSize = (strlen(defaultDir) + 1);
		params->directory_name = malloc(strSize * sizeof(char));
		if(params->directory_name == NULL)
		{
			perror("Insufficient memory");
			exit(1);
		}
		strncpy(params->directory_name, defaultDir, strSize);
	}
	return 1;
}

//Scan directory to check how many .ppm files are there
void scanDirectory(parameters* params)
{
	DIR* dirstream;
	struct dirent* direntry;

	params->n_files = 0;

	dirstream = opendir(params->directory_name);
	if(!dirstream)
	{
		perror("Could not open directory");
		exit(1);
	}
	else
	{
		while((direntry = readdir(dirstream)) != NULL)
		{
			//Count how many files exist in the directory
			if(direntry->d_type == DT_REG)
			{
				params->n_files++;
			}
		}
	}
		
	(void)closedir(dirstream);
}

//Free all parameters - avoiding memory leak
int freeParams(parameters* params)
{
	free(params->input_name);
	free(params->output_name);
	free(params->directory_name);
	return 1;
}