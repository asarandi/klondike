#include "klondike.h"

static int __ht_insert(void **entries, size_t capacity, t_board *b)
{
    size_t  i;

    i = b->hash % capacity;
    while (entries[i])
    {
        if (((t_board *)entries[i])->hash == b->hash)
            return 0;
        i = (i + 1) % capacity;
    }
    entries[i] = b;
    return 1;
}

static void ht_extend(t_hash_table *ht)
{
    size_t  new_capacity, count, i;
    void    **new_entries;

    new_capacity = ht->capacity << 1;
    if (!(new_entries = calloc(new_capacity, sizeof(void *))))
        return (void)fprintf(stderr, "%s: calloc() failed\n", __func__);
    for (i=count=0; i < ht->capacity; i++)
    {
        if (ht->entries[i]) {
            if (__ht_insert(new_entries, new_capacity, ht->entries[i]))
                ++count;
        }
        if (count >= ht->num_keys)
            break;
    }
    free(ht->entries);
    ht->entries = new_entries;
    ht->capacity = new_capacity;
}

void        ht_insert(t_hash_table *ht, t_board *b)
{
    if (((ht->num_keys * 100) / ht->capacity) > 75)
        ht_extend(ht);
    if (__ht_insert(ht->entries, ht->capacity, b))
        ++ht->num_keys;
}

t_board     *ht_find(t_hash_table *ht, t_board *b)
{
    size_t  i;

    i = b->hash % ht->capacity;
    while (ht->entries[i])
    {
        if (((t_board *)ht->entries[i])->hash == b->hash)
            return (t_board *)ht->entries[i];
        i = (i + 1) % ht->capacity;
    }
    return NULL;
}

t_hash_table *ht_init()
{
    t_hash_table    *ht;

    if (!(ht = calloc(1, sizeof(t_hash_table))))
        (void)fprintf(stderr, "%s: calloc() failed\n", __func__);
    ht->capacity = 256;
    if (!(ht->entries = calloc(ht->capacity, sizeof(void *))))
        (void)fprintf(stderr, "%s: calloc() failed\n", __func__);
    return ht;
}

void ht_destroy(t_hash_table *ht)
{
    free(ht->entries);
    free(ht);
}
