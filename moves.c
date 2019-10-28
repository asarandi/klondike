#include "klondike.h"

#define RANK(a) (((a)>>2)&15)
#define SUIT(a) ((a)&3)

/*
    stock_to_waste
    waste_to_stock
    waste_to_foundation
    waste_to_tableau
    tableau_to_foundation
    tableau_to_tableau
*/

int num_stock_to_waste(t_board *b)
{
    return strlen(b->stock) ? 1:0;
}

int num_waste_to_stock(t_board *b)
{
    return !strlen(b->stock) && strlen(b->waste);
}

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

int num_moves(t_board *b)
{
    int res = 0;

    res += num_stock_to_waste(b);
    res += num_waste_to_stock(b);
    res += num_waste_to_foundation(b);
    res += num_waste_to_tableau(b);
    res += num_tableau_to_foundation(b);
    res += num_tableau_to_tableau(b);
    return res;
}
