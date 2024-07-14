#ifndef LINKEDLIST_H
#define LINKEDLIST_H

typedef struct ListNode {
    void* data;
    struct ListNode* next;
} ListNode;

typedef struct LinkedList {
    ListNode* head;
    int count;
} LinkedList;


extern LinkedList* create_list(void);
extern void* destroy_list(LinkedList* llist);
extern void* insert_into_list(LinkedList* llist, void* data);
extern void* append_to_list(LinkedList* llist, void* data);
extern void* get_list_head(LinkedList* llist);
extern void* get_list_tail(LinkedList* llist);
extern void* find_in_list(LinkedList* llist, void* data, int(*cmpfn)(void*, void*));
extern void* delete_from_list(LinkedList* llist, void* data, int(*cmpfn)(void*, void*));
extern int get_list_count(LinkedList* llist);
extern void list_foreach(LinkedList* llist, void(*fn)(void*, void*), void* arg);
extern void **get_list_array(LinkedList* llist);
extern void free_list_array(void** arr);

#endif