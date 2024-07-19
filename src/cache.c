#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../headers/hashtable.h"
#include "../headers/cache.h"


/**
 * Create cache
 */
Cache* create_cache(int max_size, int hashsize)
{
    
}

/**
 * Allocate a cache entry
 */
CacheEntry* alloc_entry(char* path, char* content_type, void* content, int content_length)
{

}

/**
 * Deallocate a cache entry
 */
void free_entry(CacheEntry* entry)
{
    if (entry != NULL)
    {
        if (entry->content != NULL)
            free(entry->content);
        if (entry->content_type != NULL)
            free (entry->content_type);
        if (entry->path != NULL)
            free(entry->path);

        free(entry);
    }
        
}

/**
 * Insert cache entry at the head of the linked list
 */
void insert_at_dllist_head(Cache* cache, CacheEntry* entry)
{
    if (cache->head == NULL)
    {
        cache->head = cache->tail = entry;
        entry->prev = entry->next = NULL;
    }
    else {
        cache->head->prev = entry;
        entry->next = cache->head;
        entry->prev = NULL;
        cache->head = entry;
    }
}