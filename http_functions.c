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

#include "headers/config_functions.h"
#include "headers/UAcapabilities.h"
#include "headers/http_functions.h"
#include "headers/socket.h"
#include "headers/auxiliary_functions.h"
#include "headers/image_functions.h"

Header parse_http_headers(char *buf, char * extension) {

	Header head;
	int counter=0;
	head.fullHeader = buf;
	char *appoggio[10];

	int max_lines = split_lines(head.fullHeader, &appoggio);
	//for (counter=0; counter<sizeof(appoggio);counter++)
	for (counter=0; counter<max_lines;counter++)
	{

		//
		//sprintf(provaa, "%d", counter);
		//Log(provaa);

		if (counter==0)
		{
			head.request=appoggio[counter];

			//break;
		}
		else if (strstr(appoggio[counter], "Host:")!=NULL)
		{
			head.host=appoggio[counter];
			head.host=head.host+6;

			//break;
		}

		else if (strstr(appoggio[counter], "User-Agent:")!=NULL)
		{
			head.userAgent=appoggio[counter];
			head.userAgent=head.userAgent+12;

			//break;
		}

		else if (strstr(appoggio[counter], "Accept:")!=NULL)
		{
			head.acc=appoggio[counter];
			head.acc=head.acc+8;

			//break;
		}
		//else break;
		//}
	}
	//Log(head.acc);
	return head;

}



int getRequestType(char *input)
{
	// IF NOT VALID REQUEST
	// RETURN -1
	// IF VALID REQUEST
	// RETURN 1 IF GET
	// RETURN 2 IF HEAD
	// RETURN 0 IF NOT YET IMPLEMENTED
	int type = -1;
	if ( strlen ( input ) > 0 )
	{
		type = 1;
	}
	char *requestType = malloc(5);
	scan(input, requestType, 0);
	if ( type == 1 && strcmp("GET", requestType) == 0)
	{
		type = 1;
	}
	else if (type == 1 && strcmp("HEAD", requestType) == 0)
	{
		type = 2;
	}
	else if (strlen(input) > 4 && strcmp("POST", requestType) == 0 )
	{
		type = 0;
	}
	else
	{
		type = -1;
	}
	return type;
}


int handleHttpGET(char *input, char *type)
{
	char *NFpath = (char*)malloc(1000);
	char *NFfilename = (char*)malloc(200);
	char *resFilename = (char*)malloc(200);
	char *resPath = (char*)malloc(1000);

	char *filename = (char*)malloc(200);
	char *path = (char*)malloc(1000);
	char *extension = (char*)malloc(10);
	char *mime = (char*)malloc(200);
	user_agent ua;
	Header headtmp;

	char httpVersion[20];
	int contentLength = 0;
	int mimeSupported = 0;
	int fileNameLenght = 0;

	memset(NFpath, '\0', 1000);
	memset(NFfilename, '\0', 200);
	memset(path, '\0', 1000);
	memset(filename, '\0', 200);
	memset(extension, '\0', 10);
	memset(mime, '\0', 200);
	memset(httpVersion, '\0', 20);

	//sendString(input,connecting_socket);
	if (strcmp(type,"HEAD")==0)
		fileNameLenght = scan(input, filename, 6);
	else
		fileNameLenght = scan(input, filename, 5);

	if ( fileNameLenght > 0 )
	{
		if ( getHttpVersion(input, httpVersion, type) != -1 )
		{
			FILE *fp;
			FILE *newFile;
			FILE *notFound;

			if ( GetExtension(filename, extension) == -1 )
			{
				sendHeader("400 Bad Request", mime, contentLength, connecting_socket);

				char status_code[100]="400 Bad Request\n";

				Log(status_code);
				free(filename);
				free(mime);
				free(path);
				free(extension);
				return 1;
			}

			// Open the requesting file as binary //
			strcpy(path, wwwroot);
			sprintf(resPath,"%sres/",path);
			strcat(path, filename);


			sprintf(resFilename,"%s%s",resPath,filename);


			fp = fopen(path, "rb");
			Log(resFilename);

			//sendString( strcat(path,filename),connecting_socket);
			if (fp==NULL)
				fp= fopen(resFilename, "rb");
			if (fp==NULL)
			{
				strcpy(NFpath, wwwroot);
				strcat(NFpath, "404.html");
				notFound=fopen(NFpath,"rb");
				Log(NFpath);

				printf("Unable to open file");
				//sendHeader("404 Not Found", mime, contentLength, connecting_socket);
				//sendString("HTTP/1.1 404 Not Found\n", connecting_socket);
				char status_code[20]="404 Not Found\n";
				mime="text/html";
				sendHeader("404 Not Found", mime, Content_Lenght(notFound), connecting_socket);
				sendFile(notFound, Content_Lenght(notFound));
				fclose(notFound);
			    //fclose(fp);

				Log(status_code);
				//free(filename);
				//free(mime);
				//free(extension);
				//free(path);
				Log(status_code);
				//fclose(fp);

				return -1;
			}

			mimeSupported = checkMime(extension, mime);
			if ( mimeSupported != 1)
			{
				sendHeader("400 Bad Request", mime, contentLength, connecting_socket);
				char status_code[100]="Mime not supported\n 400 Bad Request\n";
				Log(status_code);
				fclose(fp);
				free(filename);
				free(mime);
				free(path);
				free(extension);
				return -1;
			}
			Log(mime);
			// Calculate Content Length //
			contentLength = Content_Lenght(fp);
			if (contentLength < 0 )
			{
				printf("File size is zero");
				free(filename);
				free(mime);
				free(extension);
				free(path);
				fclose(fp);
				return -1;
			}


			// Send File Content //
			if (strcmp(type, "HEAD")==0){
				sendHeader("200 OK", mime, contentLength, connecting_socket);
				char status_code[20]="200 OK QUESTA E UNA HEAD";
				Log(status_code);
				fclose(fp);
			}

			else if (strcmp(type, "GET")==0)
			{
				// Se è un'immagine, processa la qualità

				if (strstr(mime,"image")!=NULL)
				{

					fclose(fp);
					strcpy(path, wwwroot);
					//sprintf(resPath,"%sres/",path);

					headtmp=parse_http_headers(input, extension);
					if (headtmp.userAgent==NULL)
					ua=getUserAgentCapabilities("INVALID USER AGENT");
					else
					ua=getUserAgentCapabilities(headtmp.userAgent);
					//strcat(path,filename);
					strcat(resPath, convert(filename, ua, parseQuality(headtmp, extension), extension));
					Log(resPath);
					newFile = fopen(resPath, "rb");
					contentLength = Content_Lenght(newFile);
					sendHeader("200 OK", mime, contentLength, connecting_socket);
					char status_code[20]="200 OK";
					Log(status_code);
					sendFile(newFile, contentLength);
					fclose(newFile);
				}

				else
				{
					sendHeader("200 OK", mime, contentLength, connecting_socket);
					char status_code[20]="200 OK";
					Log(status_code);
					sendFile(fp, contentLength);
					fclose(fp);
				}

			}
			free(filename);
			free(mime);
			free(extension);
			free(path);
			return 1;
		}
		else
		{
			sendHeader("501 Not Implemented", mime, contentLength, connecting_socket);
			Log("501 Not Implemented\n");
		}
	}
	return -1;
}


int sendString(char *message, int socket)
{
	int length, bytes_sent;
	length = strlen(message);
	bytes_sent = send(socket, message, length, 0);
	return bytes_sent;
}


int sendBinary(int *byte, int length)
{
	int bytes_sent;
	bytes_sent = send(connecting_socket, byte, length, 0);
	return bytes_sent;
	return 0;
}


void sendHeader(char *Status_code, char *Content_Type, int TotalSize, int socket)
{
	char *head = "HTTP/1.1 ";
	char *content_head = "\nContent-Type: ";
	char *server_head = "\nServer: PT06";
	char *length_head = "\nContent-Length: ";
	char *date_head = "\nDate: ";
	char *newline = "\n";
	char contentLength[100];
	time_t rawtime;
	time(&rawtime);
	// int contentLength = strlen(HTML);
	sprintf(contentLength, "%i", TotalSize);
	char *message = malloc((
			strlen(head) +
			strlen(content_head) +
			strlen(server_head) +
			strlen(length_head) +
			strlen(date_head) +
			strlen(newline) +
			strlen(Status_code) +
			strlen(Content_Type) +
			strlen(contentLength) +
			28 +
			sizeof(char)) * 2);
	//if ( message != NULL )
	//{
		strcpy(message, head);
		strcat(message, Status_code);
		strcat(message, server_head);
		strcat(message, length_head);
		strcat(message, contentLength);
		strcat(message, content_head);
		strcat(message, Content_Type);
		strcat(message, date_head);
		strcat(message, (char*)ctime(&rawtime));
		strcat(message, newline);
		sendString(message, socket);
		Log(message);
		free(message);
	//}
}


void sendHTML(char *statusCode, char *contentType, char *content, int size, int socket)
{
	sendHeader(statusCode, contentType, size, socket);
	sendString(content, socket);
}


void sendFile(FILE *fdesc, int file_size)
{
	int current_char = 0;
	do{
		current_char = fgetc(fdesc);
		sendBinary(&current_char, sizeof(char));
	}
	while(current_char != EOF);
}


int checkMime(char *extension, char *mime_type)
{
	char *current_word = malloc(600);
	char *mimeList = malloc(500);
	char *word_holder = malloc(600);
	char *line = malloc(200);
	int startline = 0;

	strcat(mimeList,wwwroot);
	strcat(mimeList,"utils/mime.types");

	FILE *mimeFile;
	mimeFile=fopen(mimeList, "r");
	//if (mimeFile==NULL)

	free(mime_type);
	mime_type = (char*)malloc(200);
	memset (mime_type,'\0',200);

	while(fgets(line, 200, mimeFile) != NULL) {

		if ( line[0] != '#' )
		{
			startline = scan(line, current_word, 0);
			while ( 1 )
			{
				startline = scan(line, word_holder, startline);
				if ( startline != -1 )
				{
					if ( strcmp ( word_holder, extension ) ==0 )
					{
						memcpy(mime_type, current_word, strlen(current_word));
						free(current_word);
						free(word_holder);
						free(line);
						return 1;
					}
				}
				else
				{
					break;
				}
			}
		}
		memset (line,'\0',200);
	}
	free(current_word);
	free(word_holder);
	free(line);
	return 0;
}


int getHttpVersion(char *input, char *output, char *type)
{
	/*int result;
	strncpy(output,input+(strlen(input)-10),8);
	sendString(output, connecting_socket);
	if (strcmp(output,"HTML/1.1")==0)
			return 1;
	else if (strcmp(output,"HTML/1.0")== 0)
			return 0;
	else return -1;*/
	int tipo = 4;
	char *filename = malloc(100);
	if (strcmp(type,"HEAD")==0)
		tipo=5;

	int start = scan(input, filename, tipo);

	if ( start > 0 )
	{
		if ( scan(input, output, start) )
		{
			output[strlen(output)+1] = '\0';
			if ( strcmp("HTTP/1.1" , output) == 0 )
				return 1;
			else if ( strcmp("HTTP/1.0", output) == 0 )
				return 0;
			else
				return -1;
		}
		else
			return -1;
	}
	return -1;
}


int GetExtension(char *input, char *output)
{
	int in_position = 0;
	int appended_position = 0;
	int i = 0;
	for ( ; i < strlen(input); i ++ )
	{
		if ( in_position == 1 )
		{
			output[appended_position] = input[i];
			appended_position +=1;
		}
		if ( input[i] == '.' )
			in_position = 1;
	}
	output[appended_position+1] = '\0';
	if ( strlen(output) > 0 )
		return 1;
	return -1;
}


int Content_Lenght(FILE *fp)
{
	int filesize = 0;
	fseek(fp, 0, SEEK_END);
	filesize = ftell(fp);
	rewind(fp);
	return filesize;
}

int parseQuality(Header head, char* extension)
{
	int quality;
	char *q_start;
	char q_str[5];
	char *q_end;
	char qmime[20]="image/";
	// 18/11
	if (head.acc==NULL)
		return 100;
	if ( strstr(head.acc, "image")!=NULL)
	{
		if (strstr(head.acc,strcat(qmime,extension))!=NULL)
		q_start = strstr(head.acc, qmime);
		else
			q_start = strstr(head.acc, "image");
	}

	else if  (strstr(head.acc, "*/*")!=NULL)
		q_start = strstr(head.acc, "*/*");


	if (strstr(q_start,"q=")!=NULL){

	q_start = strstr(q_start,"q=");
	q_start=q_start+4;

	if (strstr(q_start, ",")!=NULL)
	q_end = strstr(q_start,",");
	else
	q_end = q_start;

	quality=atoi(q_end);
	return quality*10;
	}
	else return 100;
}
