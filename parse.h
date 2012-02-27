// 
// parse.h
//
// Authors: Dana Burkart (dsb3573), Gideon Williams (gaw5951)
// 
// Description: Parse parses a line of input into a command list
//

#ifndef __PARSE_H__
#define __PARSE_H__

#define C_PIPE		1
#define C_LESS		2
#define C_GREA		3
#define C_FILE 		4

#define DELIMS		" \t\n"

struct cmd {
	char	**list;                 // Tokenized command
	int		pmode;                  // Pipe mode
    int     fd_in[2];               // Input file descriptor
    int		fd_out[2];              // Output file descriptor
};

typedef struct cmd cmd;
typedef cmd* cmd_list;

cmd_list parse( char * );

size_t cmdlist_len( cmd_list );

cmd_list cmdlist_add( cmd_list, cmd );

#endif

