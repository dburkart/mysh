#include <stdlib.h>
#include <string.h>

#include "parse.h"

int list_len(void *);
cmd *cmdlist_add( cmd *, cmd* );
cmd tokenlist_add( cmd, char * );

cmd *parse_prg( char * );

cmd *parse( char *line ) {
    char *a;
    cmd *toks, *tmp;
    int pass_count = 0;
    
    a = strtok( line, " \t" );
    
    do {
       
       // Handle |
       if ( strcmp( a, "|" ) == 0 ) {

       }

       // Handle <
       else if ( strcmp( a, "<" ) == 0 ) {

       }

       // Handle >
       else if ( strcmp( a, ">" ) == 0 ) {

       }

       // Handle everything else
       else {
           tmp = parse_prg( a );
           toks = cmdlist_add( toks, tmp );
       }
        
    } while ( a != NULL );
}

int list_len( void *lst ) {
    int count = 0;
    while(lst != NULL)
           count++,lst++;
    return (count);
}

cmd *cmdlist_add( cmd *list, cmd *c ) {
    int i = 0;

    i = list_len(list);
    
    list = (cmd *)realloc( list, sizeof( cmd ) * (i + 1) );
    list[i] = *c;
    list[i+1] = (cmd)0;

    return list;
}

cmd tokenlist_add( cmd c, char *str ) {
    int i = 0;

    i = list_len(c.list);
    
    c.list = (char **)realloc( c.list, sizeof( char * ) * (i + 1) );
    c.list[i] = str;
    c.list[i+1] = 0;

    return c;
}

cmd *parse_prg( char *a ) {
    cmd *t;
    t = (cmd *)malloc( sizeof( cmd ) );

    do {
        *t = tokenlist_add( *t, a );

        a = strtok( NULL, " \t" );
    } while ( a != NULL &&
              strcmp( a, "|" ) != 0 &&
              strcmp( a, "<" ) != 0 &&
              strcmp( a, ">" ) != 0 );

    return t;
}
