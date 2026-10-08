/* Doubly Linked List interface */
#pragma once
#include <types.h>


// Each element in the list
// Make sure to manually call the free method on item, if the item is not 'wired/attached' to the list
typedef struct item {
    void* data;
    usize dataLen;
    void (*free)(struct item* it);
} item; 

// node
typedef struct _node {
    item* item;
    struct _node* next;
    struct _node* prev;
    void (*free)(struct _node* node);
} _node;

// head 
typedef struct list {
    /* Public */

	// Attributes
    usize len;
   
	// Methods
	void  (*free)            (struct list* self);
	void  (*insert)          (struct list* self, item* it, isize index, b8 replace);
	void  (*append)          (struct list* self, item* it);
	void  (*prepend)         (struct list* self, item* it);
    void  (*removeIndex)     (struct list* self, isize index);
    void  (*removeValue)     (struct list* self, item* it);
	item* (*popIndex)        (struct list* self, isize index, b8 toReturn);
	item* (*pop)             (struct list* self, b8 toReturn);
	item* (*get)             (struct list* self, isize index);
    usize (*search)          (struct list* self, item* it);
    isize (*getInvertedIndex)(struct list* self, isize index);
    void  (*clear)           (struct list* self);

    // Static Methods
    item* (*createItem)       (const void* src, usize dataLen);

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
	_node* (*_findNodeByIndex)  (struct list* self, isize index);
    void   (*_wireNode)         (struct list* self, _node* node, isize index);
    void   (*_unwireNode)       (struct list* self, _node* node);
    isize  (*_optimizeIndex)    (struct list* self, isize index);
    void   (*_checkIndexInRange)(struct list* self, isize index);

    // Static Methods
    _node* (*_createNode)         (item* it);
    item*  (*_unwireItemFromNode) (_node* node);

} list;

//! ===========================================================================================

/* 
    Doc:
        Constructor
    Error -> Sentinel based
        null => Heap Failure
*/
list* List(void);

//! ===========================================================================================

/* 
    Doc:
        Constructor
    @param arr => pointer to the array
    @param dataLen => size of each item
    @param arrLen => no. of items in array

    Error -> sentinel-based
        null => Heap Failure -> Propagated from either
                                List() constructor function as null
                                _createNode() as null
                                append() as list_HEAP_FAILURE State Machine
*/
list* List_fromArray(const void* arr, usize dataLen, usize arrLen);

//! ===========================================================================================

/*
    Doc:
        Destructor
*/
static void free_list(list* self);

//! ===========================================================================================

/*
    Doc:
        Inserts an item into an index in the list. 
        If index == length of list, it will simply append

    @param it => pointer to the item to insert
    @param index => position in list to insert to
    @param replace => If replace is true, it will replace the existing element at that index. If false, it will push the existing element one index forward

    Error -> State Machine:
        list_INDEX_OUT_OF_RANGE
        list_HEAP_FAILURE -> Propagated from _createNode as null
*/
static void insert(list* self, item* it, isize index, b8 replace);

//! ===========================================================================================

/*
    Doc:
        Appends the item in the list
    @param it => pointer to item

    Error -> State Machine
        list_HEAP_FAILURE -> Propagated from _createNode as null
*/
static void append(list* self, item* it);

//! ===========================================================================================

/*
    Doc:
        Appends the item in the list
    @param it => pointer to item

    Error -> State Machine
        list_HEAP_FAILURE -> Propagated from _createNode as null
*/
static void prepend(list* self, item* it);

//! ===========================================================================================

static void removeIndex(list* self, isize index);

//! ===========================================================================================

static void removeValue(list* self, item* it);

//! ===========================================================================================

static item* popIndex(list* self, isize index, b8 toReturn);

//! ===========================================================================================

static item* pop(list* self, b8 toReturn);

//! ===========================================================================================

static item* get(list* self, isize index);

//! ===========================================================================================

static usize search(list* self, item* it);

//! ===========================================================================================
/*
    Doc:
        Returns the index with the opposite sign, that references the same node that the given index does.
    @param index => index, to be inversed. DOES NOT VALIDATE INDEX
*/
static isize getInvertedIndex(list* self, isize index);

//! ===========================================================================================

/*
    Doc:
        Resets the list to clean state, all nodes are destroyed and metadata updated
*/
static void clear(list* self);

//! ===========================================================================================

/* 
    Doc:
        Separate util; Not a method
        Used for the free function pointer for the freeable resource 'item'
*/
static void _createItem_free(item* it);
/*
    Doc:
        Constructs the 'item' on the heap, and returns a pointer to it

    @param src => pointer to the data source (to copy into item.data)
    @param dataLen => size of data in bytes

    Error -> sentinel based:
        null => Heap Failure
*/
static item* createItem(const void* src, usize dataLen);

//! ===========================================================================================
/*
    Doc:
        Traverses through the list to reach the node, and returns a pointer to it
    @param index => index of node to return. MUST BE VALID ALREADY (<= len or corresponding -ive)

    Error -> Sentinel based
        null => The index is equal to self->len. (Safe to insert at this index, it will be an append)
*/
static _node* _findNodeByIndex(list* self, isize index);

//! ===========================================================================================

/*
    Doc:
        Attaches the node to the list at index. Pushes the existing node forward
        If index == len, it will technically append (no node to push)
        Increments the list->len
    @param node => pointer to node
    @param index => The index to attach to. MUST BE VALID ALREADY (<= len or corresponding -ive)
*/
static void _wireNode(list* self, _node* node, isize index);
//! ===========================================================================================

/*
    Doc:
        Unwires the node from the list, and properly wires the neighboring nodes to each other
        Decrements the list->len
    @param node => pointer to the node to unwire
*/
static void _unwireNode(list* self, _node* node);

//! ===========================================================================================

/*
    Doc:
        Returns an optimized index for traversing
        Index optimization; if index is closer to the opposite side of the list (2 sides because -ive indexing is supported), invert it so that the list is traversed from the closer side, saving compute & time.
    @param index => The index to be optimized
*/
static isize _optimizeIndex(list* self, isize index);

//! ===========================================================================================

/*
    Doc:
        Checks if the index is in the current range of the list
    @param index => index to be checked

    Error -> State Machine:
        list_INDEX_OUT_OF_RANGE 
*/
static void _checkIndexInRange(list* self, isize index);

//! ===========================================================================================

/* 
    Doc:
        Separate util; Not a method
        Used for the free function pointer for the freeable resource 'node'
*/
static void _createNode_free(_node* node);
/*
    Doc:
        Constructs the 'node' type on the heap, and returns a pointer to it

    @param it => pointer to the item, which will be attached to the node

    Error -> sentinel-based:
        null => Heap failure
*/
static _node* _createNode(item* it);

//! ===========================================================================================

/*
    Doc:
        Returns the pointer to the item, and sets the node->item to null (unwires it from the node)
        WARNING: Always make sure to unwire the node from list before calling this
*/
static item* _unwireItemFromNode(_node* node);
//! ===========================================================================================