     // Number of HTTP headers
int parseQuality(struct Header head, char* extension);
static char *skip(char **buf, const char *delimiters);
static int parse_http_headers(char *buf, char * extension);
int sendString(char *message, int socket);
int sendBinary(int *byte, int length);
void sendHeader(char *Status_code, char *Content_Type, int TotalSize, int socket);
void sendHTML(char *statusCode, char *contentType, char *content, int size, int socket);
void sendFile(FILE *fp, int file_size);
int scan(char *input, char *output, int start);
int checkMime(char *extension, char *mime_type);
int getHttpVersion(char *input, char *output, char *type);
int GetExtension(char *input, char *output);
int Content_Lenght(FILE *fp);
int handleHttpGET(char *input, char *type);
int getRequestType(char *input);
int receive(int socket);
void createSocket();
void bindSocket();
void startListener();
void handle(int socket);
void acceptConnection();
void start();
void initConfiguration();
void init();
int resizeImage(FILE* file, char** filename, int *quality);
void Log (char *message); // logs a message to LOGFILE
void LogErr (char *message); // logs a message; execution is interrupted
char *convertImage(char* filename, int quality);
