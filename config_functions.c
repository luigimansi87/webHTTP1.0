#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "headers/config_functions.h"
#include "headers/log.h"
#include "headers/socket.h"


	FILE *filePointer = NULL;

void init()
{

	char* currentLine = malloc(100);
	wwwroot = malloc(100);
	conf_file = malloc(100);
	log_file = malloc(100);
	mime_file = malloc(600);
	// Setting default values
	conf_file = "httpd.conf";
	//log_file = ".log";
	strcpy(mime_file, "mime.types");

	filePointer = fopen(conf_file, "r");
	// Ensure that the configuration file is open
	if (filePointer == NULL)
	{
		fprintf(stderr, "Can't open configuration file!\n");
		exit(1);
	}

	// Get server root directory from configuration file
	if (fscanf(filePointer, "%s %s", currentLine, wwwroot) != 2)
	{
		fprintf(stderr, "Error in configuration file on line 1!\n");
		exit(1);
	}
	// Get default port from configuration file
	if (fscanf(filePointer, "%s %i", currentLine, &port) != 2)
	{
		fprintf(stderr, "Error in configuration file on line 2!\n");
		exit(1);
	}

	if (fscanf(filePointer, "%s %s", currentLine, log_file) != 2)
	{
		fprintf(stderr, "Error in configuration file on line 3!\n");
		exit(1);
	}
	fclose(filePointer);
	free(currentLine);
}
