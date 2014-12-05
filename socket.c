
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

#include "headers/auxiliary_functions.h"
#include "headers/socket.h"


socklen_t addr_size;
struct sockaddr_in address;
struct sockaddr_storage connector;

int receive(int socket)
{
	int msgLen = 0;
	char buffer[BUFFER_SIZE];
	memset (buffer,'\0', BUFFER_SIZE);
	if ((msgLen = recv(socket, buffer, BUFFER_SIZE, 0)) == -1)
	{
		printf("Error handling incoming request");
		return -1;
	}
	int request = getRequestType(buffer);
	if ( request == 1 )	// GET
	{
		Log(buffer);
		handleHttpGET(buffer, "GET");
	}
	else if ( request == 2 )	// HEAD
	{
		Log(buffer);
		handleHttpGET(buffer, "HEAD");
	}
	else if ( request == 0 )	// POST
	{
		Log(buffer);
		sendString("501 Not Implemented\n", connecting_socket);
	}
	else	// GARBAGE
	{
		sendString("400 Bad Request\n", connecting_socket);
	}
	return 1;
}
/**
Create a socket and assign current_socket to the descriptor
 **/
void createSocket()
{
	current_socket = socket(AF_INET, SOCK_STREAM, 0);
	if ( current_socket == -1 )
	{
		perror("Create socket");
		exit(-1);
	}
}
/**
Bind to the current_socket descriptor and listen to the port in PORT
 **/
void bindSocket()
{
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(port);
	if ( bind(current_socket, (struct sockaddr *)&address, sizeof(address)) < 0 )
	{
		perror("Bind to port");
		exit(-1);
	}
}
/**
Start listening for connections and accept no more than MAX_CONNECTIONS in the Queue
 **/
void startListener()
{
	if ( listen(current_socket, MAX_CONNECTIONS) < 0 )
	{
		perror("Listen on port");
		exit(-1);
	}
}
/**
Handles the current connector
 **/
void handle(int socket)
{
	// --- Workflow --- //
	// 1. Receive ( recv() ) the GET / HEAD
	// 2. Process the request and see if the file exists
	// 3. Read the file content
	// 4. Send out with correct mine and http 1.1
	if (receive((int)socket) < 0)
	{
		perror("Receive");
		exit(-1);
	}
}
void acceptConnection()
{
	// signal(SIGCHLD, SIG_IGN);
	// int child_process = fork();
	addr_size = sizeof(connector);
	connecting_socket = accept(current_socket, (struct sockaddr *)&connector, &addr_size);
	if ( connecting_socket < 0 )
	{
		perror("Accepting sockets");
		exit(-1);
	}
	handle(connecting_socket);
	close(connecting_socket);
	/*
if ( child_process == 0 )
{
exit(0);
}
while (-1 != waitpid (-1, NULL, WNOHANG));
	 */	// while (-1 != waitpid (-1, NULL, WNOHANG));
}
