#ifndef KLONDIKE_H
# define KLONDIKE_H

# include <stdio.h>
# include <stdlib.h>
# include <string.h>

typedef struct s_board {
    char desc[256];
    char waste[24];
    char stock[24];
    char f[4][13];
    char t[7][19];
} t_board;

int     stoi(char *s);
char    *itos(int i);
t_board *stob(char *s);
void    print(t_board *b);

#endif
