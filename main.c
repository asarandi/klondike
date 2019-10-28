#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct s_board {
    char desc[256];
    char waste[24];
    char stock[24];
    char f[4][13];
    char t[7][19];
} t_board;

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

int num_stock_to_waste(t_board *b)
{
    return strlen(b->stock) ? 1:0;
}

int num_waste_to_stock(t_board *b)
{
    return !strlen(b->stock) && strlen(b->waste);
}

#define RANK(a) (((a)>>2)&15)
#define SUIT(a) ((a)&3)

/* return index of foundation or -1 */
int idx_foundation_for_card(t_board *b, int card)
{
    int i, len, top;

    for (i=0; i<4; i++)
    {
        len = strlen(b->f[i]);
        if ((!len) && (RANK(card) == 1))   /* ace to empty foundation */
            return i;
        top = b->f[i][len-1];            
        if ((RANK(top)+1 == RANK(card)) && (SUIT(top) == SUIT(card)))
            return i;
    }
    return -1;
}

int num_waste_to_foundation(t_board *b)
{
    int i;

    i = strlen(b->waste);
    return ((i) && (idx_foundation_for_card(b, b->waste[i-1]) != -1));
}

/* number of tableau where you can place a card */
int num_card_to_tableau(t_board *b, int card)
{
    int i, res, len, top;

    res = 0;
    for (i=0; i<7; i++)
    {
        len = strlen(b->t[i]);
        if ((!len) && (RANK(card) == 13)) /* king to empty tableau */ {
            ++res;
            continue ;
        }
        top = b->t[i][len-1];
        if ((RANK(top)-1 == RANK(card)) && ((SUIT(top)+SUIT(card))&1))
            ++res;
    }
    return res;
}

int num_waste_to_tableau(t_board *b)
{
    int i;

    if (!(i = strlen(b->waste)))
        return 0;
    return num_card_to_tableau(b, b->waste[i-1]);
}

int num_tableau_to_foundation(t_board *b)
{
    int i, res, len, card;

    res = 0;
    for (i=0; i<7; i++)
    {
        len = strlen(b->t[i]);
        if (!len) continue ;
        card = b->t[i][len-1];
        if (idx_foundation_for_card(b, card) != -1)
            ++res;
    }
    return res;
}

int num_tableau_to_tableau(t_board *b)
{
    int i, j, card, res;

    res = 0;
    for (i=0; i<7; i++)
    {
        for (j=0; b->t[i][j]; j++)
        {
            card = b->t[i][j];
            if (!(card&64))
                continue ; /* card not open */
            res += num_card_to_tableau(b, card);
        }
    }
    return res;
}






/*
    stock_to_waste
    waste_to_stock
    waste_to_foundation
    waste_to_tableau
    tableau_to_foundation
    tableau_to_tableau
*/


int main(int ac, char **av)
{
    char *deck = "4283T3215393K254Q3845213T464K3Q124413482433233T172K1T2J12322113161629471514473J3J481Q4927412Q2J2K4631491";
    t_board *b = stob(deck);
    print(b);

    return 0;
}
