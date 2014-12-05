
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "headers/image_functions.h"
#include "headers/UAcapabilities.h"
#include "headers/config_functions.h"

char *qualityAdapter(int quality)
{
	if (quality==100)
		return "";
	char* quality_str = malloc(20);
	char* command= malloc (20);
	strcpy (command," -quality ");
	sprintf(quality_str, "%d",quality);
    return strcat(command, quality_str);

}

char *sizeAdapter (int width, int height )
{
	if (width==-1 || height==-1)
		return "";
	char* width_str= malloc(20);
	char* height_str=malloc(20);
	char* command= malloc (20);
	strcpy (command," -size ");

	sprintf(width_str, "%d",width);
	sprintf(height_str, "%d",height);
	strcat(strcat(width_str,"x"), height_str);

	return strcat(command,width_str);
}

char *colorAdapter(long colors)
{
	if (colors==-1)
		return "";
	char* colors_str= malloc(75);
	char* command= malloc (20);
	strcpy (command," -colors ");
	sprintf(colors_str, "%ld",colors);

	return strcat(command, colors_str);
}

char *extensionAdapter(char* acceptedExt, char* newExtension)
{

	char *tempBuff = malloc(200);
	char *tempExtension = malloc(20);
    strcpy(tempExtension,".");
	if (strstr(acceptedExt,newExtension)!=NULL || strcmp(acceptedExt, "NULL")== 0)
		return strcat(tempExtension,newExtension);
	int i = 1;
	//tempExtension[0]='.';
		while (acceptedExt[i]!='.')
			 {
			tempExtension[i]=acceptedExt[i];
			i++;
			 }
	    tempExtension[i]='\0';

	    return tempExtension;
}

char *convert (char* fname, user_agent uatemp, int qualitytmp, char* extensions)
{
	char* stats= (char*) malloc (200);
	char* fullFn= (char*) malloc (200);
	char* newFn= (char*) malloc (200);
	char* command = (char*) malloc (1000);

	stats = generatePrefix( fname, uatemp, qualitytmp);
	newFn = createNewFilename( fname, uatemp.format, extensions);
	fullFn = strcat(stats,newFn);
	command = generateCommand(fname, fullFn, uatemp, qualitytmp);
	Log(command);
	Log(fullFn);

	return convertImage(fullFn, command);
}

char *generateCommand(char* fname, char* newFileName, user_agent uatemp, int qualitytmp)
{
	char* cmd= (char*) malloc (100);
	sprintf (cmd, "convert %s%s%s%s%s", fname, qualityAdapter(qualitytmp),sizeAdapter(uatemp.width, uatemp.height), colorAdapter(uatemp.colors), newFileName);
	Log(cmd);

	return cmd;
}

char *generatePrefix(char* fname, user_agent uatemp, int qualitytmp )
{

	char* prefix= (char*) malloc (200);

	if ( uatemp.width != -1 && uatemp.height != -1)
	sprintf(prefix,"%s%sx%s%s", returnEmptyLine(qualitytmp,1), returnEmptyLine(uatemp.width,0), returnEmptyLine(uatemp.height,0), returnEmptyLine(uatemp.colors,0));
	else
	sprintf(prefix,"%s%s%s%s", returnEmptyLine(qualitytmp,1), returnEmptyLine(uatemp.width,0), returnEmptyLine(uatemp.height,0), returnEmptyLine(uatemp.colors,0));

	Log(prefix);
	return prefix;
}

char *generateSuffix(char* format, char* extensions)
{
	return extensionAdapter(format,extensions);
}

char *createNewFilename(char* fname, char* format, char* extensions)
{
	char* newFile= (char*) malloc (200);
	int i = 0;
				while (fname[i]!='.')
				{
					newFile[i]=fname[i];
				i++;
				}
				newFile[i]='\0';

		strcat(newFile, generateSuffix(format, extensions));
		Log(newFile);
		return newFile;

}

char *returnEmptyLine(int toConv, int isQuality)
{
	char* vBuff = malloc(200);
	if (toConv == -1 && isQuality == 0 )
		return "";
	if (toConv == 100 && isQuality == 1)
		return "";
		sprintf(vBuff, "%d", toConv);
		return vBuff;
}


char *convertImage(char* filename, char* command)
{

	FILE* converted;
	FILE* check;
	char* pwd= (char*) malloc (300);
	sprintf(pwd,"%sres/", wwwroot);
	if (!(check = fopen(filename,"r")))
	{
		Log(command);
		chdir(pwd);
		converted = popen(command,"r");
		chdir(wwwroot);
		pclose(converted);
	}
	else
		fclose(check);

	//Log(newFilename);
	return filename;
}
