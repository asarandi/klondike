#include "klondike.h"

/* FNV-1a */
size_t hash(void *data, size_t size)
{
    size_t hash = 0xcbf29ce484222325L;
    for (size_t i=0; i<size; i++)
    {
        hash ^= *(unsigned char *)(data+i);
        hash *= 0x00000100000001B3L;
    }
    return hash;
}

int heuristic(t_board *b)
{

    int i, j, res, calc;
    for (i=calc=0; i<4; i++)
    {
        for (j=0; b->f[i][j]; j++)
            ++calc;
    }
    res = 52-calc;
    for (i=0;i<7;i++)
    {
        for (j=0; b->t[i][j]; j++)
            res += (!(b->t[i][j] & 64));
    }
    return res;
}

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
    snprintf(b->desc, SIZE_DESC, "initial");
    return b;
}
