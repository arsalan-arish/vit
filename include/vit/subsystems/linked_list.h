/* Doubly Linked List implementation */
#pragma once
#include <types.h>


// Each unit of data: item
typedef struct {
    void* data;
    usize dataLen;
} item; 

// node
typedef struct node {
    item item;
    struct node* next;
    struct node* prev;
} node;

// head 
typedef struct {
    node* first;
    node* last;
    usize len;
    struct {
        b8 is;
        enum {
          list_INDEX_OUT_OF_RANGE = 1,
          list_POP_FROM_EMPTY_LIST,
          list_VALUE_NOT_FOUND,
          list_HEAP_FAILURE,
        } code;
    } err; // if (err.is) check is convenient
} list;


// TODO:
//* Create standardized comments for all functions, and create comments for functions that can raise an error to the state machine
//* Clean up the codebase (rewrite from scratch)
//* Write tests

/* Convention (Simulating OOP in C)
    Functions starting with List_ are constructors
    Function with name list_free is the destructor
    Functions are namespace prefixed with list_
    Internal API Functions are prefixed with _list_ 
    Functions accepting list* parameter are stateful methods
    Functions not accepting list* are stateless utils (and static associated functions only)
*/

list List(void);
list List_fromArray(list* list, u8* arr, usize dataLen, usize arrLen);

void list_free(list* list);

item list_append(list* list, usize dataLen);
item list_prepend(list* list, usize dataLen);
item list_insert(list* list, usize dataLen, i64 index, b8 replace);

item list_pop(list* list, b8 toReturn);
item list_popIndex(list* list, i32 index, b8 toReturn);
void list_removeValue(list* list, void* data, usize dataLen);
void list_removeIndex(list* list, i64 index); 

item list_get(list* list, i64 index);
usize list_search(list* list, void* data, usize dataLen);
isize list_getInvertedIndex(list* list, i64 index);

/*
    _list_createNode: Internal stateless function 

    Doc:
        Constructs the 'node' type on the heap, and returns a pointer to it

    @param dataBufferPtr => An arbitrary pointer to a 'pointer location', where the address of the buffer will be written
    @param dataLen     => The size of the databuffer
    @return node*

    Error -> return sentinel-based:
        null => Heap failure (the OS did not give the requested memory)
*/
node* _list_createNode(void** dataBufferPtr, usize dataLen);
item _list_createItem(void** dataBufferPtr, usize dataLen);

void _list_createNode_free(node* node);
void _list_createItem_free(item item); 

void _list_extractNodeFromList(list* list, node* node);
item _list_extractItemFromNode(node* node);

node* _list_findNodeByIndex(list* list, i64 index);
isize _list_optimizeIndex(list* list, i64 index);
void _list_checkIndexInRange(list* list, i64 index);