#ifndef KLONDIKE_H
# define KLONDIKE_H

# include <stdio.h>
# include <limits.h>
# include <stdlib.h>
# include <string.h>

typedef struct s_board
{
    int             c;
    int             g;
    int             h;
    size_t          hash;
    struct s_board  *parent;
    char            desc[256];
    char            waste[24 + 1];
    char            stock[24 + 1];
    char            f[4][13 + 1];
    char            t[7][19 + 1];
} t_board;

int                 stoi(char *s);
char                *itos(int i);
t_board             *stob(char *s);
void                print(t_board *b);
int                 num_moves(t_board *b);
t_board             **make_moves(t_board *b);
size_t              hash(void *data, size_t size);
int                 heuristic(t_board *b);

#endif
