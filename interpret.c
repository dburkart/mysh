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
void printcmd( cmd, char * );

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


		if (cmdlist_len(stack) == 0 && c.pmode) {
			printcmd(c, "waiting on C");
			waitpid( result, NULL, 0 );
			printcmd(c, "wait finished");
		} else {

			switch (c.pmode) {
				case C_PIPE:
					a = cmdlist_pop();
					b = cmdlist_pop();

					printcmd( c, "pipe(c)" );
					printcmd( a, "	   a" );
					printcmd( b, "	   b" );

					if ( a.pmode ) {
						cmdlist_push( c );
					} else {
						pipe( b.fd_in );
						cmdlist_push( b );
					}

					result = fork();
					//----in child past this point------

					if (result == 0) {
						//our input should either be a pipe or stay as stdin(0)
						//our output should either be a pipe or remain stdout(1)
						//I think we are only doing a OR b here not both.
						if ( a.pmode ) {
							close( 0 );
							dup( a.fd_out[0] );

							if ( c.fd_out[1] != 1 ) {
								close( 1 );
								dup( c.fd_out[1] );
							}

							execvp( b.list[0], b.list );
							//no longer in our program, no code past here
						} else {
							close( 1 );
							dup( b.fd_in[1] );
							close( b.fd_in[0] );

							execvp( a.list[0], a.list );
							//no longer in our program, no code past here
						}
					} else if (result == -1) {

					}

					break;

				case C_LESS:
					a = cmdlist_pop();
					b = cmdlist_pop();

					printcmd( c, "less(c)" );
					printcmd( a, "	   a" );
					printcmd( b, "	   b" );

					result = fork();

					if (result == 0) {
						//our input should be a file(freopen)
						//our output should be a pipe or stay as stdout(1)
						//close( 0 );
						freopen(b.list[0], "r", stdin);

						if ( c.fd_out[1] != 1 ) {
							close( 1 );
							dup( c.fd_out[1] );
						}

						execvp( a.list[0], a.list );

					} else if (result == -1) {

					}

					cmdlist_push( c );

					break;

				case C_GREA:
					a = cmdlist_pop();
					b = cmdlist_pop();

					printcmd( c, "great(c)" );
					printcmd( a, "	   a" );
					printcmd( b, "	   b" );

					result = fork();

					if (result == 0) {
						//our input should either be from c or stdin(0)
						//our output should be a file(line below)
						close( 0 );
						freopen(b.list[0], "a", stdout);

						if ( c.fd_out[1] != 1 ) {
							close( 1 );
							dup( c.fd_out[1] );
						}

						execvp( a.list[0], a.list );

					} else if (result == -1) {

					} else {
						waitpid( result, NULL, 0 );
					}

					break;

				default:
					printcmd( c, "execute" );

					result = fork();

					if (result == 0) {
						if (c.fd_in[0] != 0) {
							close( 0 );
							dup( c.fd_in[0] );
							close( c.fd_in[1] );
						}

						if (c.fd_out[1] != 1) {
							close( 1 );
							dup( c.fd_out[1] );
							close( c.fd_out[0] );
						}

						execvp( c.list[0], c.list );
					} else if (result == -1) {
						perror( "Fork blew up\n" );
					} else {
						waitpid( result, NULL, 0 );
					}

					break;
			}
		}

		stack_swap();
		c = cmdlist_pop();
	} while (c.list != NULL || c.pmode != 0 );

	stack_size = 0;
	free( stack );
	stack = NULL;

}

cmd_list reverse_polish( cmd_list cmds ) {
	size_t len = cmdlist_len( cmds );
	stack = NULL;
	cmd prev = (cmd){0, 0, {0, 1}, {-1, -1}};

	// Reverse the command list
	for ( int i = len-1; i >= 0; i-- ) {
		if ( cmds[i].pmode ) {
			if ( prev.pmode ) {
				pipe( cmds[i].fd_out );
				prev.fd_in[0] = cmds[i].fd_out[0];
				prev.fd_in[1] = cmds[i].fd_out[1];
			}
			prev = cmds[i];
		}

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

void printcmd( cmd c, char *where) {
	printf( "%s: pmode: %d, fd_in: {%d, %d}, fd_out: {%d, %d}\n", where, c.pmode,
			c.fd_in[0], c.fd_in[1], c.fd_out[0], c.fd_out[1] );
}
