/// TODO:
//* Clean up the codebase (rewrite from scratch)
//* Write tests

/* Doubly Linked List interface */
#pragma once
#include <types.h>


// Each element in the list
typedef struct item {
    void* data;
    usize dataLen;
    void (*free)(struct item it);
} item; 

// node
typedef struct _node {
    item item;
    struct node* next;
    struct node* prev;
    void (*free)(struct _node node);
} _node;

// head 
typedef struct list {
    /* Public */

	// Attributes
    usize len;
   
	// Methods
	void  (*free)            (struct list* self);
	item  (*append)          (struct list* self, item it);
	item  (*prepend)         (struct list* self, item it);
	item  (*insert)          (struct list* self, item it, isize index, b8 replace);
	item  (*pop)             (struct list* self, b8 toReturn);
	item  (*popIndex)        (struct list* self, isize index, b8 toReturn);
    item  (*removeIndex)     (struct list* self, isize index);
    void  (*removeValue)     (struct list* self, item it);
	item  (*get)             (struct list* self, isize index);
    usize (*search)          (struct list* self, item it);
    isize (*getInvertedIndex)(struct list* self, isize index);
    void  (*clear)           (struct list* self);

    // Static Methods
    item (*createItem)       (const void* copyFromBuf, usize dataLen);

    // Error Struct (State Machine Mechanism of Error-Signalling)
    struct {
        b8 is;
        enum {
          list_INDEX_OUT_OF_RANGE,
          list_POP_FROM_EMPTY_LIST,
          list_VALUE_NOT_FOUND,
          list_HEAP_FAILURE,
        } code;
    } err;

    /* Private */

    // Attributes
    _node* _first;
    _node* _last;

	// Methods
	_node* (*_findNodeByIndex)     (struct list* self, isize index);
    isize  (*_optimizeIndex)       (struct list* self, isize index);
    void   (*_checkIndexInRange)   (struct list* self, isize index);
    void   (*_extractNodeFromList) (struct list* self, _node* node); 

    // Static Methods
    _node* (*_createNode)         (item it);
    item  (*_extractItemFromNode) (_node* node);

} list;


/* Separate utility stub function */
void _attachFuncPtrs(list* list);

/* 
    Error -> Sentinel based
        null => Heap Failure
*/
list* List(void);

/* 
    Error -> Sentinel based
        null => Heap Failure
*/
list* List_fromArray(const void* arr, usize dataLen, usize arrLen);

static void free_list(list* self);
static item append(list* self, item it);
static item prepend(list* self, item it);
static item insert(list* self, item it, isize index, b8 replace);
static item pop(list* self, b8 toReturn);
static item popIndex(list* self, isize index, b8 toReturn);
static item removeIndex(list* self, isize index);
static void removeValue(list* self, item it);
static item get(list* self, isize index);
static usize search(list* self, item it);
static isize getInvertedIndex(list* self, isize index);
static void clear(list* self);

static item createItem(const void* copyFromBuf, usize dataLen);

static _node* _findNodeByIndex(list* self, isize index);
static isize _optimizeIndex(list* self, isize index);
static void  _checkIndexInRange(list* self, isize index);
static void  _extractNodeFromList(list* self, _node* node);

static _node* _createNode(item it);
static item  _extractItemFromNode(_node* node);