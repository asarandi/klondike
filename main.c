#include "klondike.h"

int is_goal(t_board *b)
{
    int         i, j, calc;

    for (i=calc=0; i<4; i++)
    {
        for (j=0; b->f[i][j]; j++)
            ++calc;
    }
    return calc == 52;
}

int cmp(void *ptr_a, void *ptr_b)
{
    int         res;
    t_board     *a, *b;

    a = ptr_a;
    b = ptr_b;
    res = (a->g + a->h) - (b->g + b->h);
    return res == 0 ? a->c - b->c : res;
}

void p(t_board *b)
{
    int count = 0;
    while (b)
    {
        ++count;
        print_board(b);
        printf("\n");
        b = b->parent;
    }
    printf("\ntotal = %d moves\n\n\n\n\n\n", count);
    fflush(stdout);
}

void ht_free_all(t_hash_table *ht)
{
    size_t  i;

    for (i=0; i<ht->capacity; i++)
    {
        if (ht->entries[i])
            free(ht->entries[i]);
    }
}

int main(int ac, char **av)
{
    pq          *q;
    t_hash_table *ht;
    t_board     *root, *vertex, *tmp, **moves;
//    char        *deck = "4283T3215393K254Q3845213T464K3Q124413482433233T172K1T2J12322113161629471514473J3J481Q4927412Q2J2K4631491";
    char        *deck = "2264Q27282629433Q454529184232131837324Q343K39213K412T2J26174716334448141J4T1K2J111T493J3Q1T35342321451K1";
    int         i, saved_g = 0, tentative_g;

    if ((ac == 2) && (strlen(av[1]) == 52<<1))
        deck = av[1];
    printf("new game!\n%s\n\n", deck); fflush(stdout);
    root = stob(deck);
    root->h = heuristic(root);

    ht = ht_init();
    ht_insert(ht, root);

    q = pq_init(&cmp);
    pq_insert(q, root);

    while (q->num_nodes)
    {
        vertex = (t_board *)pq_extract(q);
        if (is_goal(vertex))
            break ;
        if (vertex->is_closed)
            continue ;
        vertex->is_closed = 1;
        tentative_g = vertex->g + 0; /* XXX */
        moves = make_moves(vertex);
        if (!moves)
            continue ;
        for (i=0; moves[i]; i++)
        {
            if ((tmp = ht_find(ht, moves[i])))
            {
                free(moves[i]);
                moves[i] = tmp;
            }
            if (moves[i]->is_closed)
                continue ;
            if ((moves[i]->is_open) && (moves[i]->g <= tentative_g))
                continue ;
            moves[i]->is_open = 1;
            moves[i]->g = tentative_g;
            moves[i]->parent = vertex;

            if (!tmp)
                moves[i]->h = heuristic(moves[i]);

            ht_insert(ht, moves[i]);
            pq_insert(q, moves[i]);
        }
        free(moves);
    }
    p(vertex);
    ht_free_all(ht);
    ht_destroy(ht);
    pq_destroy(q);
    return 0;
}
