#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "../headers/linkedlist.h"


/**
 * Routine for allocating a new linked list
 */
LinkedList* create_list(void)
{
    // Allocate and zero-initialize memory for a new linked list (just one node, the head)
    return calloc(1, sizeof(LinkedList));
}

/**
 * Free memory occupied by a linked list
 */
void* destroy_list(LinkedList* llist)
{
    ListNode* node = llist->head, *next_node;
    while (node != NULL)
    {
        next_node = node->next;
        free(node);
        node = next_node;
    }

    free(llist);
}

/** 
* Insert a node at the head of a linked list 
*/
void* insert_into_list(LinkedList* llist, void* data)
{
    ListNode* node = calloc(1, sizeof(*node));

    if (node == NULL)
        return NULL;    // Memory allocation failed.
    
    node->data = data;
    node->next = llist->head;
    llist->head = node;
    
    llist->count++;

    return data;
}

/**
 * Append to end of a linked list
 */
void *append_to_list(LinkedList* llist, void* data)
{
    ListNode* tail = llist->head;

    if (tail == NULL)
        return insert_into_list(llist, data);
    
    ListNode* node = calloc(1, sizeof(*node));

    while(tail->next != NULL)
    {
        tail = tail->next;
    }
    
    node->data = data;
    tail->next = node;

    llist->count++;

    return data;
}

/**
 * Return first element in a linked list
 */
void* get_list_head(LinkedList* llist)
{
    if (llist->head == NULL)
        return NULL;
    
    return llist->head->data;
}

/**
 * Return last element in a linked list
 */
void* get_list_tail(LinkedList* llist)
{
    ListNode* node = llist->head;

    if (node == NULL)
        return NULL;

    while(node->next != NULL)
        node = node->next;
    
    return node->data;
}

/**
 * Search for an element in a linked list
 */
void* find_in_list(LinkedList* llist, void* data, int(*cmpfn)(void*, void*))
{
    ListNode* node = llist->head;

    if (node == NULL)
        return NULL;

    while (node != NULL)
    {
        if (cmpfn(data, node->data) == 0)
            break;

        node = node->next;
    }

    return node->data;
}

/**
 * Delete element from linked list
 */
void* delete_from_list(LinkedList* llist, void* data, int(*cmpfn)(void*, void*))
{
    ListNode*  node = llist->head, *prev = NULL;

    while(node != NULL)
    {
        if (cmpfn(data, node->data) == 0)
        {
            void *data = node->data;

            if (prev == NULL)
            {
                llist->head = node->next;
                free(node);
            }
            else {
                prev->next = node->next;
                free(node);
            }

            llist->count--;

            return data;
        }

        prev = node;
        node = node->next;
    }

    return NULL;
}

/**
 * Return the number of elements in the list
 */
int get_list_count(LinkedList* llist)
{
    return llist->count;
}

/**
 * ForEach itertion of linked list
 */
void list_foreach(LinkedList* llist, void (*fn)(void*, void*), void* arg)
{
    ListNode* np = llist->head, *next;

    while (np != NULL)
    {
        next = np->next;
        fn(np->data, arg);
        np = next;
    }
}

/**
 * Allocate and return a NULL-terminated array of pointers to elements in a linked list
 */

void** get_list_array(LinkedList* llist)
{
    if (llist->head == NULL)
        return NULL;

    void **arr = malloc(sizeof(*arr) * llist->count + 1);

    ListNode* node;
    int i;

    for (i = 0, node = llist->head; node != NULL; i++, node = node->next)
        arr[i] = node->data;
    
    arr[i] = NULL;    // Null terminate array.

    return arr;
}

/**
 * Free array allocated with get_list_array
 */
void free_list_array(void **arr)
{
    // Only frees the array, does not modify the contents of the linked list.
    free(arr);
}