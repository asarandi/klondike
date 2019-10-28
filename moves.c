#include "klondike.h"

#define RANK(a) (((a)>>2)&15)
#define SUIT(a) ((a)&3)

static void *g_moves[32];

static void store_move(void *src, void *dst)
{
    int i;

    if (((!src) || (!dst)) || (src == dst))
        return ;
    i = 0;
    while (g_moves[i])
    {
        if ((g_moves[i] == src) && (g_moves[i + 1] == dst))
            return ;
        i += 2;            
    }
    g_moves[i] = src;
    g_moves[i + 1] = dst;
    g_moves[i + 2] = NULL;
}

static void stock_to_waste(t_board *b)
{
    int i, j;
    
    if (!(i = strlen(b->stock)))
        return ;
    j = strlen(b->waste);
    store_move(&b->stock[i - 1], &b->waste[j]);
}

static void waste_to_stock(t_board *b)
{
    if (strlen(b->stock))
        return ;
    if (!strlen(b->waste))
        return ;
    store_move(&b->waste, &b->stock); /* XXX: reverse order */
}

static void to_foundation(t_board *b, void *ptr_card)
{
    int i, len, top, card;

    card = *(char *)ptr_card;
    for (i=0; i<4; i++)
    {
        if (!(len = strlen(b->f[i]))) {
            if (RANK(card) == 1)    /* ace to empty foundation */
                return store_move(ptr_card, &b->f[i][len]);
            continue ;                
        }
        top = b->f[i][len-1];            
        if ((RANK(top)+1 == RANK(card)) && (SUIT(top) == SUIT(card)))
            return store_move(ptr_card, &b->f[i][len]);
    }
}

static void waste_to_foundation(t_board *b)
{
    int i;

    if (!(i = strlen(b->waste)))
        return ;
    return to_foundation(b, &b->waste[i-1]);
}

static void to_tableau(t_board *b, void *ptr_card)
{
    int i, len, top, card;

    card = *(char *)ptr_card;
    for (i=0; i<7; i++)
    {
        if (!(len = strlen(b->t[i]))) {
            if (RANK(card) == 13) /* king to empty tableau */
                store_move(ptr_card, &b->t[i][len]);
            continue;
        }
        top = b->t[i][len-1];
        if ((RANK(top)-1 == RANK(card)) && ((SUIT(top)+SUIT(card))&1))
            store_move(ptr_card, &b->t[i][len]);
    }
}

static void waste_to_tableau(t_board *b)
{
    int i;

    if (!(i = strlen(b->waste)))
        return ;
    return to_tableau(b, &b->waste[i-1]);
}

static void tableau_to_foundation(t_board *b)
{
    int i, len;

    for (i=0; i<7; i++)
    {
        len = strlen(b->t[i]);
        if (!len) continue ;
        to_foundation(b, &b->t[i][len-1]);
    }
}

static void tableau_to_tableau(t_board *b)
{
    int i, j;

    for (i=0; i<7; i++)
    {
        for (j=0; b->t[i][j]; j++)
        {
            if (!((b->t[i][j])&64))
                continue ;
            to_tableau(b, &b->t[i][j]);                
        }
    }
}

int num_moves(t_board *b)
{
    int i;

    (void)memset(g_moves, 0, sizeof(g_moves));
    stock_to_waste(b);
    waste_to_stock(b);
    waste_to_foundation(b);
    waste_to_tableau(b);
    tableau_to_foundation(b);
    tableau_to_tableau(b);
    for (i=0; g_moves[i]; i+=2) ;
    return i>>1;
}

static t_board *clone(t_board *b)
{
    t_board *res;

    if (!(res = calloc(1, sizeof(t_board))))
        fprintf(stderr, "%s: calloc() failed\n", __func__);
    memcpy(res, b, sizeof(t_board));        
    strcpy(res->desc, "clone");
    return res;
}

/* field name */
static char *fn(t_board *b, void *ptr)
{
    static char *waste = "waste";
    static char *stock = "stock";
    static char *tab_0 = "tableau #1";
    static char *tab_1 = "tableau #2";
    static char *tab_2 = "tableau #3";
    static char *tab_3 = "tableau #4";
    static char *tab_4 = "tableau #5";
    static char *tab_5 = "tableau #6";
    static char *tab_6 = "tableau #7";
    static char *fnd_0 = "foundation #1";
    static char *fnd_1 = "foundation #2";
    static char *fnd_2 = "foundation #3";
    static char *fnd_3 = "foundation #4";
    
    if ((ptr >= (void *)b->waste) && (ptr < (void *)b->waste + sizeof(b->waste)))      return waste;
    if ((ptr >= (void *)b->stock) && (ptr < (void *)b->stock + sizeof(b->stock)))      return stock;        
    if ((ptr >= (void *)b->t[0]) && (ptr < (void *)b->t[0] + sizeof(b->t[0])))        return tab_0;
    if ((ptr >= (void *)b->t[1]) && (ptr < (void *)b->t[1] + sizeof(b->t[1])))        return tab_1;
    if ((ptr >= (void *)b->t[2]) && (ptr < (void *)b->t[2] + sizeof(b->t[2])))        return tab_2;
    if ((ptr >= (void *)b->t[3]) && (ptr < (void *)b->t[3] + sizeof(b->t[3])))        return tab_3;
    if ((ptr >= (void *)b->t[4]) && (ptr < (void *)b->t[4] + sizeof(b->t[4])))        return tab_4;
    if ((ptr >= (void *)b->t[5]) && (ptr < (void *)b->t[5] + sizeof(b->t[5])))        return tab_5;
    if ((ptr >= (void *)b->t[6]) && (ptr < (void *)b->t[6] + sizeof(b->t[6])))        return tab_6;
    if ((ptr >= (void *)b->f[0]) && (ptr < (void *)b->f[0] + sizeof(b->f[0])))        return fnd_0;
    if ((ptr >= (void *)b->f[1]) && (ptr < (void *)b->f[1] + sizeof(b->f[1])))        return fnd_1;
    if ((ptr >= (void *)b->f[2]) && (ptr < (void *)b->f[2] + sizeof(b->f[2])))        return fnd_2;
    if ((ptr >= (void *)b->f[3]) && (ptr < (void *)b->f[3] + sizeof(b->f[3])))        return fnd_3;

    fprintf(stderr, "%s: error: should not be here\n", __func__);
    return NULL;
}

static void move(t_board *original, t_board *clone, int idx)
{
    int i, len, tmp;
    char *src, *dst;

    src = (void *)g_moves[idx] - (void *)original + (void *)clone;
    dst = (void *)g_moves[idx+1] - (void *)original + (void *)clone;
    snprintf(clone->desc, 256, "move %s from %s to %s", itos(*src), fn(clone,src), fn(clone,dst));
    len = strlen(src);
    strncpy(dst, src, len);
    memset(src, 0, len);
    for (i=0; i<7; i++) {
        if (len = strlen(clone->t[i]))
            clone->t[i][len-1] |= 64;
    }    
    if ((void *)dst != (void *)clone->stock)
        return ;
    snprintf(clone->desc, 256, "move waste to stock");
    len = strlen(clone->stock);
    for (i=0; i<(len>>1); i++) {
        tmp = dst[i];
        dst[i] = dst[len-1-i];
        dst[len-1-i] = tmp;
    }
}

t_board **make_moves(t_board *b)
{
    int i, n;
    t_board **res;

    if (!(n = num_moves(b)))
        return NULL;
    if (!(res = calloc(n + 1, sizeof(t_board *))))
        fprintf(stderr, "%s: calloc() failed\n", __func__);
    res[n] = NULL;        
    for (i=0; g_moves[i]; i += 2)
    {
        res[i>>1] = clone(b);
        move(b, res[i>>1], i);
    }
    return res;
}
