#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../headers/linkedlist.h"
#include "../headers/hashtable.h"

#define DEFAULT_SIZE 128
#define DEFAULT_GROW_FACTOR 2

// Linked List cleanup
struct foreach_callback_payload {
    void* arg;
    void (*fn)(void*, void*);
};


/**
 * Change entry count, maintain load metric
 */
void add_entry_count(HashTable* hashtable, int n)
{
    hashtable->num_entries += n;
    hashtable->load = (float)hashtable->num_entries / hashtable->size;
}

/**
 * Default (basic) modulo hashing function
 */
int default_hashfn(void* data, int data_size, int bucket_count)
{
    const int R = 31;   // prime number
    int hval = 0;
    unsigned char* p = data;
    
    for (int i = 0; i < data_size; i++)
        hval = (R * hval + p[i]) % bucket_count;

    return hval;
}

/**
 * Create a new hashtable
 */
HashTable* create_hashtable(int size, int(*hashfn)(void*, int, int))
{
    if (size < 1)
        size = DEFAULT_SIZE;
    
    if (hashfn == NULL)
        hashfn = default_hashfn;

    HashTable* hashtable = malloc(sizeof(*hashtable));

    if (hashtable == NULL)
        return NULL;    // Memory allocation failed.
    
    hashtable->size = size;
    hashtable->num_entries = 0;
    hashtable->load = 0;
    hashtable->bucket = malloc(sizeof(LinkedList) * size);
    hashtable->hashfn = hashfn;

    for (int i = 0; i < size; i++)
        hashtable->bucket[i] = create_list();

    return hashtable;
}

/**
 * Routine for freeing a hashtable entry
 */
void free_hashtabe_entry(void* hashtable_entry, void* arg)
{
    (void)arg;
    free (hashtable_entry);
}

/**
 * Destroy a hashtable
 */
void destroy_hashtable(HashTable* hashtable)
{
    for (int i = 0; i < hashtable->size; i++)
    {
        LinkedList* llist = hashtable->bucket[i];

        list_foreach(llist, free_hashtabe_entry, NULL);

        destroy_list(llist);
    }

    free(hashtable);
}

/**
 * Add an item to the hashtable
 */
void* put_into_hashtable(HashTable* hashtable, char* key, void* data)
{
    return put_bin_into_hashtable(hashtable, key, strlen(key), data);
}

/**
 * Add binary data to hashtable
 */
void* put_bin_into_hashtable(HashTable* hashtable, void* key, int key_size, void* data)
{
    int ndx = hashtable->hashfn(key, key_size, hashtable->size);

    LinkedList* llist = hashtable->bucket[ndx];

    HashTableEntry* entry = malloc(sizeof(*entry));
    entry->key = malloc(key_size);
    memcpy(entry->key, key, key_size);
    entry->key_hash = ndx;
    entry->data = data;

    if (append_to_list(llist, entry) == NULL)
    {
        free(entry->key);
        free(entry);
        return NULL;
    }

    add_entry_count(hashtable, 1);

    return data;
}

/**
 * Comparison function for hastable entries
 */
int hashtablecmp(void* a, void* b)
{
    HashTableEntry *entryA = a, *entryB = b;

    int size_diff = entryB->key_size - entryA->key_size;

    if (size_diff)
        return size_diff;
    
    return memcmp(entryA->key, entryB->key, entryA->key_size);
}

/**
 * Get entry from hashtable with string key
 */
void* get_from_hashtable(HashTable* hashtable, char* key)
{
    return get_bin_from_hastable(hashtable, key, strlen(key));
}

/**
 * Get binary data from hashtable
 */
void* get_bin_from_hastable(HashTable* hashtable, void* key, int key_size)
{
    int ndx = hashtable->hashfn(key, key_size, hashtable->size);

    LinkedList* llist = hashtable->bucket[ndx];

    HashTableEntry cmpentry;
    cmpentry.key = key;
    cmpentry.key_size = key_size;

    HashTableEntry* entry = find_in_list(llist, &cmpentry, hashtablecmp);

    if (entry == NULL)
        return NULL;
    
    return entry->data;
}

/**
 * Delete item from hashtable by string key
 */
void * delete_from_hash_table(HashTable* hashtable, char* key)
{
    return delete_bin_from_hash_table(hashtable, key, strlen(key));
}

void* delete_bin_from_hash_table(HashTable* hashtable, void* key, int key_size)
{
    int ndx = hashtable->hashfn(key, key_size, hashtable->size);

    LinkedList* llist = hashtable->bucket[ndx];

    HashTableEntry cmpentry;
    cmpentry.key = key;
    cmpentry.key_size = key_size;

    HashTableEntry* entry = delete_from_list(llist, &cmpentry, hashtablecmp);

    if (entry == NULL)
        return NULL;
    
    void* data = entry->data;

    free(entry);

    add_entry_count(hashtable, -1);

    return data;
}

/**
 * Foreach callback
 */
void foreach_callback(void* vent, void* vpayload)
{
    HashTableEntry* entry = vent;
    struct foreach_callback_payload* payload = vpayload;

    payload->fn(entry->data, payload->arg);
}

/**
 * Foreach for elements in a hashtable
 */
void hashtable_foreach(HashTable* hashtable, void (*fn)(void*, void*), void* arg)
{
    struct foreach_callback_payload payload;

    payload.fn = fn;
    payload.arg = arg;

    
    for (int i = 0; i < hashtable->size; i++)
    {
        LinkedList* llist = hashtable->bucket[i];

        list_foreach(llist, foreach_callback, &payload);
    }
}