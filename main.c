#include "klondike.h"

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

int is_goal(t_board *b)
{
    int i, j, calc;
    for (i=calc=0; i<4; i++)
    {
        for (j=0; b->f[i][j]; j++)
            ++calc;
    }
    return calc == 52;
}

int cmp(t_board *a, t_board *b)
{
    return memcmp(a,b,sizeof(t_board));
}

int is_seen(t_board **path, t_board *node)
{
    for (int i=0; path[i]; i++)
    {
        if (!cmp(path[i], node))
            return 1;
    }
    return 0;
}

int search(t_board **path, int *idx, int g, int bound)
{
    static int p;

    t_board *vertex = path[*idx];
    int f = g + heuristic(vertex);
    if (f > bound)
        return f;
    if (is_goal(vertex))
        return 0x7ffffffa;
    int min = 0x7fffffff;
    t_board **moves = make_moves(vertex);
    if (!moves) return min;
    for (int i=0; moves[i]; i++)
    {
        if (is_seen(path, moves[i]))
            continue ;
        path[++(*idx)] = moves[i];
        int t = search(path, idx, g + 1, bound);
        if (t == 0x7ffffffa)
            return 0x7ffffffa;
        if (t < min) {
            min = t;
            if (p < t)
            {
                p = t;
                for (int j=0;j<10;j++) printf("- - -\n");
                printf("idx = %d, new bound = %d\n", *idx, t);
                for (int j=0;j<=*idx;j++) print(path[j]);
            }
        }
        free(path[*idx]);            
        path[(*idx)--] = NULL;            
    }
    free(moves);
    return min;
}

int main()
{

//    char *deck = "4283T3215393K254Q3845213T464K3Q124413482433233T172K1T2J12322113161629471514473J3J481Q4927412Q2J2K4631491";
    char *deck = "2264Q27282629433Q454529184232131837324Q343K39213K412T2J26174716334448141J4T1K2J111T493J3Q1T35342321451K1";
    static t_board *path[256];
    t_board *root = stob(deck);

    int bound = heuristic(root);
    int t;
    path[0] = root;
    int idx = 0;
    for (;;)
    {
        t = search(path, &idx, 0, bound);
        if (t == 0x7ffffffa)
            break ;
        if (t == 0x7fffffff)
            break ;
        bound = t;
    }
    printf("result: %08x\n", t);
    return 0;
}
