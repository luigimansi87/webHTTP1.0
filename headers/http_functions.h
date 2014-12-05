#ifndef HTTP_FUNCTIONS_H
#define HTTP_FUNCTIONS_H


typedef struct
{
	char *fullHeader;
	char *host;
	char *acc;
	char *userAgent;
	char *language;
	char *request;
} Header;

Header parse_http_headers(char *buf, char * extension);
int getRequestType(char *input);
int handleHttpGET(char *input, char *type);
int sendString(char *message, int socket);
int sendBinary(int *byte, int length);
void sendHeader(char *Status_code, char *Content_Type, int TotalSize, int socket);
void sendHTML(char *statusCode, char *contentType, char *content, int size, int socket);
void sendFile(FILE *fdesc, int file_size);
int checkMime(char *extension, char *mime_type);
int getHttpVersion(char *input, char *output, char *type);
int GetExtension(char *input, char *output);
int Content_Lenght(FILE *fp);
int parseQuality(Header head, char* extension);

#endif
