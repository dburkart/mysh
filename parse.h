#ifndef __PARSE_H__
#define __PARSE_H__

#define C_PIPE      1
#define C_LESS      2
#define C_GREA      3

union cmd {
    char    **list;
    int     pmode;
};

typedef union cmd cmd;

cmd *parse( char *line );

#endif

