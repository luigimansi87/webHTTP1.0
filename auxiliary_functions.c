#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "headers/auxiliary_functions.h"
#include "headers/socket.h"
#include "headers/log.h"


int split_lines(char *buffer, char **line_buffer)
{
	size_t cur_line, i;
	char *l_start, *l_end;

	i = 0;
	cur_line = 0;
	l_start = buffer;
	l_end = buffer;

	//while ('\0' != l_end[1]) {
		while ( '\0' != l_end[1]) {
		/* Increment pointer to end of line after assignment. */
		l_end++;
		if ('\n' == l_end[0]) {

			/* We don't want to process empty lines. */
			if ((size_t)(l_end - l_start) > 1) {
				line_buffer[cur_line] = calloc(750,
						sizeof(char));
				if (NULL == line_buffer[cur_line])
					return -1;

				/*
				 * STRCPY is a macro that expands to strncpy on linux
				 * and strlcpy on real systems.
				 */
				strncpy(line_buffer[cur_line++], l_start,
						(size_t)(l_end - l_start));
				l_start = ++l_end;
			}
			else
				/* Look for double newlines and skip them. */
				if ((1 == (size_t)(l_end - l_start)) &&
						(l_start[0] == l_end[0]))
					//l_start = ++l_end;
					return cur_line;
		}
	}

	return cur_line;
}

int scan(char *input, char *output, int start)
{
	if ( start >= strlen(input) )
		return -1;
	int appending_char_count = 0;
	int i = start;
	for ( ; i < strlen(input); i ++ )
	{
		if ( *(input + i) != '\t' && *(input + i) != ' ' && *(input + i) != '\n' && *(input + i) != '\r')
		{
			*(output + appending_char_count) = *(input + i ) ;
			appending_char_count += 1;
		}
		else
			break;
	}
	*(output + appending_char_count) = '\0';
	// Find next word start
	i += 1;
	for (; i < strlen(input); i ++ )
	{
		if ( *(input + i ) != '\t' && *(input + i) != ' ' && *(input + i) != '\n' && *(input + i) != '\r')
			break;
	}
	return i;
}


void start()
{
	createSocket();
	bindSocket();
	startListener();
	while ( 1 )
	{
		acceptConnection();
	}
}




/*static char *skip(char **buf, const char *delimiters) {
	char *p, *begin_word, *end_word, *end_delimiters;
	begin_word = *buf;
	end_word = begin_word + strcspn(begin_word, delimiters);
	end_delimiters = end_word + strspn(end_word, delimiters);
	for (p = end_word; p < end_delimiters; p++) {
		*p = '\0';
	}
	*buf = end_delimiters;
	return begin_word;
}

*/
