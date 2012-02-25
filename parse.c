#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "parse.h"

char *curr_chunk;

size_t tokenlist_len(char **);
size_t cmdlist_len( cmd_list );

cmd_list cmdlist_add( cmd_list, cmd );
cmd tokenlist_add( cmd, char * );

cmd parse_prg( char * );

cmd *parse( char *line ) {
	cmd_list toks=NULL;
	cmd tmp;

	curr_chunk = strtok( line, DELIMS );

	do {
		tmp.list = 0, tmp.pmode = 0;
		
		// Handle |
		if ( strcmp( curr_chunk, "|" ) == 0 ) {
			tmp.pmode = C_PIPE;
			toks = cmdlist_add( toks, tmp );
			
			curr_chunk = strtok( NULL, DELIMS );
		}

		// Handle <
		else if ( strcmp( curr_chunk, "<" ) == 0 ) {
			tmp.pmode = C_LESS;
			toks = cmdlist_add( toks, tmp );
			
			curr_chunk = strtok( NULL, DELIMS );
		}

		// Handle >
		else if ( strcmp( curr_chunk, ">" ) == 0 ) {
			tmp.pmode = C_GREA;
			toks = cmdlist_add( toks, tmp );
			
			curr_chunk = strtok( NULL, DELIMS );
		}

		// Handle everything else
		else {
			tmp = parse_prg( curr_chunk );
			toks = cmdlist_add( toks, tmp );
		}

	} while ( curr_chunk != NULL );

	return toks;
}

size_t tokenlist_len( char **list ) {
	size_t count = 0;

	if ( list == NULL ) return 0;

	while( *list != 0 ) {
		count++;
		list++;
	}

	return (count);
}

size_t cmdlist_len( cmd_list list ) {
	size_t count = 0;

	if ( list == NULL ) return 0;

	while( list->list != 0 ) {
		count++;
		list++;
	}

	return (count);
}

cmd_list cmdlist_add( cmd_list list, cmd c ) {
	size_t len = 0;
	size_t new_size;

	len = cmdlist_len(list);

	new_size = (sizeof(cmd) * (len + 1)) + sizeof(cmd);
	
	list = (cmd_list)realloc( list, new_size );
	list[len] = c;
	list[len+1].list = 0, list[len+1].pmode = 0;

	return list;
}

void cmdlist_free( cmd_list cmds ) {
	size_t len;
	
	len = cmdlist_len( cmds );
	
	for ( int i = 0; i < len; i++ ) {
		free( cmds[i].list );
	}
	
	free( cmds );
}

cmd tokenlist_add( cmd c, char *str ) {
	int i = 0;
	size_t new_size;

	i = tokenlist_len(c.list);
	new_size = (sizeof(char **) * (i + 1)) + (sizeof(char **));

	c.list = (char **)realloc( c.list, new_size );
	c.list[i] = str;
	c.list[i+1] = NULL;

	return c;
}

cmd parse_prg( char *a ) {
	cmd c;

	while ( a != NULL && strcmp( a, "|" ) != 0 && strcmp( a, "<" ) != 0 &&
			strcmp( a, ">" ) != 0 ) {
		c = tokenlist_add( c, a );

		a = strtok( NULL, DELIMS );
	}

	curr_chunk = a;

	return c;
}
