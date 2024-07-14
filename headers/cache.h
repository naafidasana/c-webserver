#ifndef CACHE_H
#define CACHE_H

typedef struct CacheEntry {
    char* path;
    char* content_type;
    int content_length;
    void* content;

    struct CacheEntry *prev, *next; // Doubly-linked list
} CacheEntry;


typedef struct Cache {
    struct HashTable *ndx;
    CacheEntry* head, *tail;
    int max_size;
    int cur_size;
} Cache;


extern CacheEntry *alloc_entry(char* path, char* content_type, void* content, int content_lenght);
extern void free_entry(CacheEntry* entry);
extern Cache* create_cache(int max_size, int hashsize);
extern void free_cache(Cache* cache);
extern void put_into_cache(Cache* cache, char* path, char* content_type, void* content, int content_length);
extern CacheEntry* get_from_cache(Cache* cache, char* path);

#endif