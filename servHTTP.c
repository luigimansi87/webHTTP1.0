#define ARRAY_SIZE(array) (sizeof(array) / sizeof(array[0]))
#define fseeko64 fseek
#define fopen64 fopen

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <pthread.h>
#include <sys/wait.h>
#include <time.h>


//#include "webHTTP.h"
#include "headers/auxiliary_functions.h"
#include "headers/log.h"
#include "headers/http_functions.h"
#include "headers/image_functions.h"
#include "headers/socket.h"
#include "headers/config_functions.h"
//#include "../headers/UAcapabilities.h"

int main(int argc, char* argv[])
{
	int parameterCount;
	//char* fileExt = malloc(10);
	//char* mime_type = malloc(800);
	init();
	createLog(log_file);
	for (parameterCount = 1; parameterCount < argc; parameterCount++)
	{
		// If flag -p is used, set port
		if (strcmp(argv[parameterCount], "-p") == 0)
		{
			// Indicate that we want to jump over the next parameter
			parameterCount++;
			printf("Setting port to %i\n", atoi(argv[parameterCount]));
			port = atoi(argv[parameterCount]);
		}
		// If flag -d is used, set deamon to TRUE;

		else if (strcmp(argv[parameterCount], "-l") == 0)
		{
			// Indicate that we want to jump over the next parameter
			parameterCount++;
			printf("Setting logfile = %s\n", argv[parameterCount]);
			log_file = (char*)argv[parameterCount];
		}
		else
		{
			printf("Usage: %s [-p port] [-l logfile]\n", argv[0]);
			printf("\t\t-p port\t\tWhich port to listen to.\n");
			printf("\t\t-l logfile\tWhich file to store the log to.\n");
			return -1;
		}
	}
	printf("Settings:\n");
	printf("Port:\t\t\t%i\n", port);
	printf("Server root:\t\t%s\n", wwwroot);
	printf("Configuration file:\t%s\n", conf_file);
	printf("Logfile:\t\t%s\n", log_file);

	chdir(wwwroot);
	start();
	return 0;
}


