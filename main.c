#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "parse.h"

#define BUFF_SIZE	10

char *get_line( FILE *fd );

int main() {
	char *line;
	cmd_list commands;

	while ( 1 ) {
		printf("\n? ");

		line = get_line( stdin );

		commands = parse( line );
		
		if(line != NULL){
			free( line );
		}
	}
	return 0;
}

char *get_line( FILE *fd ) {
	char buf[BUFF_SIZE];
	char *line;
	line = 0;

	int reads = 0;
	int n_read =0;
do{
	fgets( buf, BUFF_SIZE, fd );
		n_read = strlen(buf);
		reads += n_read;
		line = (char *)realloc(line, reads);

		if (line == 0) {
			printf("Error: realloc failed(newsize:%d)n",reads);
			break;
		}

		strcat(line, buf);
	} while(n_read == BUFF_SIZE-1 && buf[0] != '\n');

	return line;
}
