#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "interpret.h"
#include "parse.h"

#define BUFF_SIZE 100

cmd_list reverse_polish();

cmd_list cmdlist_push( cmd );
cmd cmdlist_pop();
void stack_swap();

cmd_list stack;
size_t stack_size = 0;

void interpret( cmd_list cmds ) {
	reverse_polish( cmds );
	cmd a, b, c;
	char buff[BUFF_SIZE];
	pid_t result;
	FILE *fp, *fd;
	int next;

	char *pipestr[1], *lessstr[1], *greastr[1];
	pipestr[0] = "PIPE-OUTPUT";
	lessstr[0] = "READ-OUTPUT";
	greastr[0] = "GREA-OUTPUT";

	c = cmdlist_pop();
	do {

		if (c.fd[0] != -1) {
//			fd = fdopen( c.fd[0], "r" );

//			do {
//				next = fgetc( fd );
//				fputc(next, stdout );
//			} while( next != EOF );

//			fclose(fd);
		} else {

		switch (c.pmode) {
			case C_PIPE:
				a = cmdlist_pop();
				b = cmdlist_pop();

				printf( "PIPING '%s*' INTO '%s*'\n", a.list[0], b.list[0] );

				cmdlist_push( (cmd){pipestr, 0, {0, 1}} );
				break;

			case C_LESS:
				a = cmdlist_pop();
				b = cmdlist_pop();

				//printf( "READING '%s*' INTO '%s*'\n", b.list[0], a.list[0] );

//				pipe(a.fd);
				if ( cmdlist_len(stack) > 0 ) {
				pipe(c.fd);
				} else {
					c.fd[0] = stdin;
					c.fd[1] = stdout;
				}

				result = fork();

				if (result == 0) {
//					close( 0 );
//					dup( a.fd[0] );
					freopen(b.list[0], "r", stdin);
//					close( c.fd[0] );
//					close( 1 );
//					dup( c.fd[1] );
//					close( c.fd[1] );

					execvp( a.list[0], a.list );
				} else if (result == -1) {

				} else {
//					fp = fopen( b.list[0], "r" );
					//fd = fdopen( a.fd[1], "w" );

//					while ( fread(buff, 1, BUFF_SIZE, fp) == BUFF_SIZE ) {
						//fread( fp, buff, BUFF_SIZE );
						//putc(next, stdout);
						//putc('\n', stdout);
						//fputc( next, fd );
//						write( a.fd[1], buff, BUFF_SIZE );
//					}

					//waitpid( result, NULL, NULL );
//					fclose(fp);
				}

				//cmdlist_push( (cmd){lessstr, 0, 0} );
				cmdlist_push( c );
				break;

			case C_GREA:
				a = cmdlist_pop();
				b = cmdlist_pop();

				printf( "WRITING '%s*' INTO '%s*'\n", a.list[0], b.list[0] );
				break;

			default:
				result = fork();

				if (result == 0) {
					execvp( c.list[0], c.list );
				} else if (result == -1) {
					perror( "Fork blew up\n" );
				} else {
					waitpid( result, NULL, NULL );
				}

				break;
		}
		}

		stack_swap();
		c = cmdlist_pop();
	} while (c.list != NULL || c.pmode != NULL);

	stack_size = 0;
	free( stack );
	stack = NULL;

}

cmd_list reverse_polish( cmd_list cmds ) {
	size_t len = cmdlist_len( cmds );
	stack = NULL;

	// Reverse the command list
	for ( int i = len-1; i >= 0; i-- ) {
		cmdlist_push( cmds[i] );
	}

	stack_swap();

	return stack;
}

cmd_list cmdlist_push( cmd c ) {
	size_t len = cmdlist_len( stack );

	if ( len < stack_size ) {
		stack[ len ] = c;
	} else {
		stack = cmdlist_add( stack, c );
		stack_size++;
	}

	return stack;
}

void stack_swap() {
	size_t len = cmdlist_len( stack );

	if ( len <= 2 ) return;

	cmd c = stack[len-1];

	stack[len-1] = stack[len-2];
	stack[len-2] = c;
}

cmd cmdlist_pop() {
	size_t len, new_size;
	cmd c;

	len = cmdlist_len( stack );
	if ( !len ) {
		return (cmd){0, 0, {-1, -1}};
	}

	c = stack[ len - 1 ];
	stack[ len - 1] = (cmd){0, 0, {-1, -1}};

	return c;
}
