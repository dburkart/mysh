#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "interpret.h"
#include "parse.h"

cmd_list reverse_polish();

cmd_list cmdlist_push( cmd );
cmd cmdlist_pop();
void stack_swap();

cmd_list stack;
size_t stack_size = 0;

void interpret( cmd_list cmds ) {
	reverse_polish( cmds );
	cmd a, b, c;
	pid_t result;

	char *pipestr[1], *lessstr[1], *greastr[1];
	pipestr[0] = "PIPE-OUTPUT";
	lessstr[0] = "READ-OUTPUT";
	greastr[0] = "GREA-OUTPUT";

	c = cmdlist_pop();
	do {

		switch (c.pmode) {
			case C_PIPE:
				a = cmdlist_pop();
				b = cmdlist_pop();

				printf( "PIPING '%s*' INTO '%s*'\n", a.list[0], b.list[0] );

				cmdlist_push( (cmd){pipestr, 0, 0} );
				break;

			case C_LESS:
				a = cmdlist_pop();
				b = cmdlist_pop();

				//printf( "READING '%s*' INTO '%s*'\n", b.list[0], a.list[0] );

				pipe(a.fd);
				pipe(b.fd);

				result = fork();

				if (result == 0) {

				} else if (result == -1) {

				} else {

				}

				cmdlist_push( (cmd){lessstr, 0, 0} );
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
		return (cmd){0, 0, 0};
	}

	c = stack[ len - 1 ];
	stack[ len - 1] = (cmd){0, 0, 0};

	return c;
}
