#ifndef HASHTABLE_H
#define HASHTABLE_H

#pragma once

typedef struct HashTable {
    int size;
    int num_entries;
    float load;
    struct LinkedList** bucket;
    int (*hashfn)(void* data, int data_size, int bucket_count);
} HashTable;


typedef struct HashTableEntry {
    void* key;
    int key_size;
    int key_hash;
    void* data;
} HashTableEntry;


extern HashTable* create_hashtable(int size, int (*hashfn)(void*, int, int));
extern void destroy_hashtable(HashTable* hashtable);
extern void* put_into_hashtable(HashTable* hashtable, char* key, void* data);
extern void* put_bin_into_hashtable(HashTable* hashtable, void* key, int key_size, void* data);
extern void* get_from_hashtable(HashTable* hashtable, char* key);
extern void* get_bin_from_hastable(HashTable* hashtable, void* key, int key_size);
extern void* delete_from_hash_table(HashTable* hashtable, char* key);
extern void* delete_bin_from_hash_table(HashTable* hashtable, void* key, int key_size);
extern void hashtable_for_each(HashTable* hashtable, void(*fn)(void*, void*), void* arg);

#endif