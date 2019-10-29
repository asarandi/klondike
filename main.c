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
    while (b)
    {
        print_board(b);
        printf("\n");
        b = b->parent;
    }
}

t_board *pick_one(t_hash_table *ht, int g)
{
    t_board *res;
    size_t  i;

    res = NULL;
    for (i=0; i<ht->capacity; i++)
    {
        if ((ht->entries[i]) && (((t_board *)ht->entries[i])->g == g))
        {
            if ((!res) || (cmp(res, ht->entries[i]) > 0))
                res = ht->entries[i];
        }
    }
    return res;
}

void ht_find_dupes(t_hash_table *ht)
{
    size_t i, j;
    void *ptr;

    for (i=0; i<ht->capacity; i++)
    {
        if (!ht->entries[i]) continue ;
        ptr = ht->entries[i];
        for (j=0; j<ht->capacity; j++)
        {
            if (!ht->entries[j]) continue ;
            if (j == i) continue;
            if (ptr != ht->entries[j]) continue ;
            printf("i = %lu, j = %lu, ptr = %p\n", i, j, ptr);
            print_board((t_board *)ptr);
        }
    }

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

void restart(t_hash_table **ht, pq **q, int g)
{
    t_board *vertex, *clone;

    vertex = pick_one(*ht, g);
    p(vertex);
    printf("\nrestart\n");
    clone = calloc(1, sizeof(t_board));
    memcpy(clone, vertex, sizeof(t_board));
    clone->g = 0;
    clone->is_closed = 0;
    clone->parent = NULL;
    ht_free_all(*ht);
    ht_destroy(*ht);
    pq_destroy(*q);

    *ht = ht_init();
    *q = pq_init(&cmp);
    pq_insert(*q, clone);
    ht_insert(*ht, clone);
}

int main()
{
    pq          *q;
    t_hash_table *ht;
    t_board     *root, *vertex, *tmp, **moves;
//    char        *deck = "4283T3215393K254Q3845213T464K3Q124413482433233T172K1T2J12322113161629471514473J3J481Q4927412Q2J2K4631491";
    char        *deck = "2264Q27282629433Q454529184232131837324Q343K39213K412T2J26174716334448141J4T1K2J111T493J3Q1T35342321451K1";
    int         i, saved_g = 0, tentative_g;

    root = stob(deck);
    root->h = heuristic(root);

    ht = ht_init();
    q = pq_init(&cmp);
    pq_insert(q, root);

    while (q->num_nodes)
    {
        if (ht->num_keys > (1<<27))
        {
            restart(&ht, &q, saved_g-2);
            saved_g = 0;
        }

        vertex = (t_board *)pq_extract(q);
        if (vertex->g > saved_g)
        {
            saved_g = vertex->g;
            printf("saved_g = %d\n",saved_g);
        }

        if (is_goal(vertex))
            break ;
        if (vertex->is_closed)
            continue ;            
        vertex->is_closed = 1;
        tentative_g = vertex->g + 1; /* XXX */
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
            ht_insert(ht, moves[i]);
            pq_insert(q, moves[i]);                            
        }
        free(moves);
    }

    p(vertex);            

    free(root);
    ht_free_all(ht);
    ht_destroy(ht);
    pq_destroy(q);

    return 0;

}
