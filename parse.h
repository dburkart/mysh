#ifndef __PARSE_H__
#define __PARSE_H__

#define C_PIPE		1
#define C_LESS		2
#define C_GREA		3

#define DELIMS		" \t\n"

struct cmd {
	char	**list;
	int		pmode;
};

typedef struct cmd cmd;
typedef cmd* cmd_list;

cmd_list parse( char *line );

#endif

