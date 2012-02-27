#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "parse.h"
#include "interpret.h"

#define BUFF_SIZE	1000

char *get_line( FILE *fd );

int main() {
	char *line;
	cmd_list commands;

	while ( 1 ) {
		printf("\n? ");

		line = get_line( stdin );

		commands = parse( line );
		if( commands == NULL ) continue;

		interpret( commands );

		if(line != NULL) {
			free( line );
			free( commands );
			line = NULL;
		}
	}
	return 0;
}

char *get_line( FILE *fd ) {
	char buf[BUFF_SIZE];
	char *line = NULL;

	int reads = 0;
	int n_read =0;
	do {
		fgets( buf, BUFF_SIZE, fd );
		n_read = strlen(buf);
		reads += n_read;
		line = (char *)realloc(line, reads + 1 );

		if (reads < BUFF_SIZE) memset( line, 0, reads + 1);

		if (line == 0) {
			printf("Error: realloc failed(newsize:%d)n",reads);
			break;
		}

		strcat(line, buf);
	} while(n_read == BUFF_SIZE-1 && buf[0] != '\n');

	return line;
}
