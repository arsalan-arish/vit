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
          LIST_INDEX_OUT_OF_RANGE = 1,
          LIST_POP_FROM_EMPTY_LIST,
          LIST_VALUE_NOT_FOUND,
          LIST_HEAP_FAILURE,
        } code;
    } err; // if (err.is) check is convenient
} list;


//*TODO: Fix Interface Problems
//* The error raising functions do not indicate so
//* The freeable resource returning functions do not indicate so, nor are the freeing functions provided following the naming convention

// Constructors
list List(void);
list List_fromArray(list* list, u8* arr, usize dataLen, usize arrLen);

// These three insertion functions return pointers to buffers of the size specified.
// Then the user writes their actual data into the buffer
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

void list_free_item(item item); 
void list_free_list(list* list);
void list__free_node(node* node);

node* list__createNode(usize dataLen, void** data_buffer);
node* list__findNodeByIndex(list* list, i64 index);
isize list__optimizeIndex(list* list, i64 index);
void list__extractNodeFromList(list* list, node* node);
item list__extractItemFromNode(node* node);
void list__checkIndexInRange(list* list, i64 index);