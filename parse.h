#ifndef __PARSE_H__
#define __PARSE_H__

#define C_PIPE		1
#define C_LESS		2
#define C_GREA		3
#define C_FILE 		4

#define DELIMS		" \t\n"

struct cmd {
	char	**list;
	int		pmode;
    int     fd_in[2];
    int		fd_out[2];
};

typedef struct cmd cmd;
typedef cmd* cmd_list;

cmd_list parse( char * );

size_t cmdlist_len( cmd_list );

cmd_list cmdlist_add( cmd_list, cmd );

void cmdlist_free( cmd_list );

#endif

