#include "klondike.h"

int stoi(char *s)
{
    char *ranks = "_123456789TJQK";
    int res = strchr(ranks, s[0]) - ranks;
    res = (res << 2) | (s[1] - '1');
    return res;
}

char *itos(int i)
{
    static char s[4];
    char *ranks = "_123456789TJQK";
    s[0] = i ? ranks[(i>>2)&15]:' ';
    s[1] = i ? (i & 3) + '1':' ';
    s[2] = i & 64 ? '.':' ';
    s[3] = 0;
    return (char *)&s;
}

t_board     *stob(char *s)
{
    t_board *b;
    int      y,x,i;

    b = calloc(1, sizeof(t_board));
    if (!b)
        fprintf(stderr, "%s: calloc() failed\n", __func__);
    for (y=i=0; y<7; y++) {
        for (x=y; x<7; x++) {
            b->t[x][y] = x == y ? 64 | stoi(s+i): stoi(s+i);
            i += 2;
        }
    }
    for (y=0; y<24; y++)
        b->stock[y] = stoi(s+i+(y<<1));
    strcpy(b->desc, "initial");
    return b;
}
