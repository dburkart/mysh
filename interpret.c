#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#include "interpret.h"

cmd_list commands;

void interpret( cmd_list cmds ){
	int roop = 0;
	int c = count_non_pmode( cmds );
	int result = 0;
	commands = (cmd_list) malloc( c * sizeof( cmd ) );
	
	setup_pipes( cmds );

	for( roop = 0; roop < c; roop++) {
		if ( cmds[roop].pmode == C_FILE ) 
			continue;
		
		result = fork();
		switch( result ) {
			case 0:
					//child

					if ( cmds[roop].fd_in[0] == -1) {
							freopen( cmds[roop-1].list[0], "r", stdin );
					}else if ( cmds[roop].fd_in[0] != 0  ) {
							//not reg stdin
							close( 0 );
							dup( cmds[roop].fd_in[0] );
					}

					if ( cmds[roop].fd_out[1] == -1 ) { 
							freopen( cmds[roop+1].list[0], "a", stdout );
					}else if ( cmds[roop].fd_out[1] != 1 ) {
							//not reg stdout
							close( 1 );
							dup( cmds[roop].fd_out[1] );
					}
					execvp( cmds[roop].list[0], cmds[roop].list);
				break;
			case -1:
				//error
				perror("Spork: ");
				break;
			default:
				break;
		}
	}
	//fsync(stdout);
	//fflush(stdout);

	//fsync(stdin);
	//fflush(stdin);

	// grep hangs so we just let the 
	//waitpid( result, NULL, 0 );
	free( commands );
}

void setup_pipes( cmd_list c ) { 
	int len = cmdlist_len( c );
	int i = 0;
	int j = 0;

	for( i = 0, j = 0; i < len; i++) { 
		switch ( c[i].pmode ) {
			case C_PIPE:
				pipe( c[i-1].fd_out );
				c[i+1].fd_in[0] = c[i-1].fd_out[0];
				//ins c i-1 into commads
				commands[j++] = c[i-1];
				break;
			case C_LESS:
				c[i-1].fd_in[0] = -1;
				//ins c i+1 into commads
				c[i+1].pmode = C_FILE;
				commands[j++] = c[i+1];
				c[i+1] = c[i-1];
				break;
			case C_GREA:
				c[i-1].fd_out[1] = -1;
				//ins c i+1 into commads
				c[i+1].pmode = C_FILE;
				commands[j++] = c[i-1];
				break;
			default:
				if( len == 1)
						commands[j++] = c[i]; 
				break;
		}
	}
	if ( j < len )
		commands[j++] = c[i-1];
	
}

int count_non_pmode( cmd_list c ) {
	int i = 0;
	int ret = 0;
	for( i = 0; c[i].list != NULL || c[i].pmode != 0;i++) {
		if ( !c[i].pmode ) ret++;
	}
	printf("Len: %d\n", ret);
	return ret;
}

void printcmd( cmd c, char *where) {
		printf( "%s: pmode: %d, fd_in: {%d, %d}, fd_out: {%d, %d}\n", where, c.pmode,
						c.fd_in[0], c.fd_in[1], c.fd_out[0], c.fd_out[1] );
}

