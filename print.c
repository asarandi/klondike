#include "klondike.h"

static int len_bf(t_board *b)
{
    int res = 0, i, j;
    for (i=0; i<4; i++)
    {
        for (j=0; b->f[i][j]; j++) ;
        res = j > res ? j : res;
    }
    return res;
}

static int len_bt(t_board *b)
{
    int res = 0, i, j;
    for (i=0; i<7; i++)
    {
        for (j=0; b->t[i][j]; j++) ;
        res = j > res ? j : res;
    }
    return res;
}

void print(t_board *b)
{
    int i, j;
    printf("%s\n", b->desc);
    printf("foundations\n");
    for (i=0; i<len_bf(b); i++) {
        for (j=0;j<4;j++)
            printf("%s%s", itos(b->f[j][i]), j+1<4?" ":"\n");
    }
    printf("waste\n");
    for (i=0; b->waste[i]; i++)
        printf("%s%s", itos(b->waste[i]), b->waste[i+1] ? " ":"\n");
    printf("stock\n");
    for (i=0; b->stock[i]; i++)
        printf("%s%s", itos(b->stock[i]), b->stock[i+1] ? " ":"\n");
    printf("tableau\n");
    for (i=0; i<len_bt(b); i++) {
        for (j=0;j<7;j++)
            printf("%s%s", itos(b->t[j][i]), j+1<7?" ":"\n");
    }
}
