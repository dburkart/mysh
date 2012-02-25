#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "parse.h"

char *curr_chunk;

int tokenlist_len(char **);
int cmdlist_len( cmd_list );

cmd *cmdlist_add( cmd *, cmd* );
cmd *tokenlist_add( cmd *, char * );

cmd *parse_prg( char * );

cmd *parse( char *line ) {
    cmd_list toks=NULL, tmp;
    int pass_count = 0;
    
    curr_chunk = strtok( line, " \t\n" );

    do {

       // Handle |
       if ( strcmp( curr_chunk, "|" ) == 0 ) {

       }

       // Handle <
       else if ( strcmp( curr_chunk, "<" ) == 0 ) {

       }

       // Handle >
       else if ( strcmp( curr_chunk, ">" ) == 0 ) {

       }

       // Handle everything else
       else {
           tmp = parse_prg( curr_chunk );
           toks = cmdlist_add( toks, tmp );
       }
       
    } while ( curr_chunk != NULL );

    return toks;
}

int tokenlist_len( char **list ) {
    int count = 0;
    
    if ( list == NULL ) return 0;

    while( *list != 0 ) {
        count++;
        list++;
    }

    return (count);
}

int cmdlist_len( cmd_list list ) {
	int count = 0;
    
    if ( list == NULL ) return 0;

    while( list->list != 0 ) {
        count++;
        list++;
    }

    return (count);
}

cmd_list cmdlist_add( cmd_list list, cmd *c ) {
    int i = 0;

    i = cmdlist_len(list);

    list = (cmd_list)realloc( list, (sizeof(cmd) * (i + 1)) + sizeof(cmd *) );
    list[i] = *c;
    list[i+1].list = 0, list[i+1].pmode = 0;

    return list;
}

cmd *tokenlist_add( cmd *c, char *str ) {
    int i = 0;
    size_t new_size;
    
    if ( str == NULL ) return NULL;

    i = tokenlist_len(c->list);
    new_size = (sizeof(char **) * (i + 1)) + (sizeof(char **));
	
    c->list = (char **)realloc( c->list, new_size );
    c->list[i] = str;
    c->list[i+1] = NULL;

    return c;
}

cmd *parse_prg( char *a ) {
    cmd *t;
    t = (cmd *)memset(malloc( sizeof( cmd ) ), 0, sizeof( cmd ) );

    while ( a != NULL && strcmp( a, "|" ) != 0 && strcmp( a, "<" ) != 0 &&
            strcmp( a, ">" ) != 0 ) {
        tokenlist_add( t, a );

        a = strtok( NULL, " \t\n" );
    }
    
    curr_chunk = a;

    return t;
}
