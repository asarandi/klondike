#ifndef KLONDIKE_H
# define KLONDIKE_H

# include <stdio.h>
# include <unistd.h>
# include <limits.h>
# include <stdlib.h>
# include <string.h>

# define SIZE_DESC 48

typedef struct      s_board
{
    int             c;
    int             g;
    int             h;
    int             is_closed;
    int             is_open;
    size_t          hash;
    struct s_board  *parent;
    char            desc[SIZE_DESC];
    char            waste[24 + 1];
    char            stock[24 + 1];
    char            f[4][13 + 1];
    char            t[7][19 + 1];
} t_board;

    int             stoi(char *s);
    char            *itos(int i);
    t_board         *stob(char *s);
    void            print_board(t_board *b);
    int             num_moves(t_board *b);
    t_board         **make_moves(t_board *b);
    size_t          hash(void *data, size_t size);
    int             heuristic(t_board *b);

/* pq */

typedef struct      s_priority_queue {
    size_t          capacity;
    size_t          num_nodes;
    int             (*cmp)(void *, void *);
    void            **nodes;
} pq;

    pq              *pq_init(int (*cmp)(void *, void *));
    void            pq_insert(pq *q, void *node);
    void            *pq_extract(pq *q);
    void            pq_destroy(pq *q);

/* ht */

typedef struct      s_hash_table {
    size_t          capacity;
    size_t          num_keys;
    void            **entries;
} t_hash_table;

    void            ht_insert(t_hash_table *ht, t_board *b);
    t_board         *ht_find(t_hash_table *ht, t_board *b);
    t_hash_table    *ht_init();
    void            ht_destroy(t_hash_table *ht);

#endif
