/*
    Doc:
        Default Constructor
*/
list List(void);

/*
    Doc:
        Constructor to initialize the list from an array
    @param arr     => pointer to start of array
    @param dataLen => the size in bytes of individual element
    @param arrLen  => the number of elements in the array
*/
list List_fromArray(u8* arr, usize dataLen, usize arrLen);

/*
    Doc:
        Destructor
*/
static void free(list* self);

/*
    Doc:
        Method that constructs an item, append it to the list and returns a copy of it for the user to write to
	@param dataLen => The length of the data buffer to be allocated for the item
	@return item   => The item that was created and added to the list
*/
static item append(list* self, usize dataLen);

/*
	Doc:
		Method that constructs an item, prepends it to the list and returns a copy of it for the user to write to
	@param dataLen => The length of the data buffer to be allocated for the item
	@return item   => The item that was created and added to the list
*/
static item prepend(list* self, usize dataLen);

/*
    Doc:
        Method that constructs an item, inserts it to the list at the specified index and returns a copy of it for the user to write to
    @param dataLen => The length of the data buffer to be allocated for the item
    @param index   => The index at which the item should be inserted
    @param replace => If true, the item at the specified index will be replaced with the new item. If false, the new item will be inserted at the specified index and all subsequent items will be shifted to the right.
    @return item   => The item that was created and added to the list
*/
static item insert(list* self, usize dataLen, i64 index, b8 replace);

/*
    Doc:
        Method that removes the last item from the list and returns it to the user. The user is responsible for freeing the data buffer of the item.
    @param toReturn => If true, the item will be returned to the user. If false, the item will be freed and not returned.
    @return item   => The item that was removed from the list
*/
static item pop(list* self, b8 toReturn);

/*
    Doc:
        Remove and return (optionally) the item at the specified index.

    Behavior:
        - If toReturn is true, the removed item is returned to the caller and the caller is responsible for freeing item.data.
        - If toReturn is false, the removed node and its data buffer are freed and a sentinel item (data = NULL, dataLen = 0) is returned.

    Error:
        - If index is out of range, list->err.is is set and INDEX_OUT_OF_RANGE is recorded.
        - If the list is empty, list->err.is is set and POP_FROM_EMPTY_LIST is recorded.

    @param index    => Index of the item to remove. Negative indices may be supported (see list semantics).
    @param toReturn => Whether to return the removed item (true) or free it (false).
    @return item    => The removed item (or sentinel on failure/when not returned).
*/
static item popIndex(list* self, i32 index, b8 toReturn);

/*
    Doc:
        Remove the first item whose data matches the provided buffer.

    Behavior:
        - Performs a value comparison using the provided dataLen bytes.
        - On success the matching node is removed and its memory freed.
        - If multiple equal values exist only the first occurrence is removed.

    Error:
        - If no matching value is found, list->err.is is set and VALUE_NOT_FOUND is recorded.

    @param data     => Pointer to the data to match.
    @param dataLen  => Number of bytes to compare.
*/
static void removeValue(list* self, void* data, usize dataLen);

/*
    Doc:
        Remove the item at the specified index without returning it to the caller.

    Behavior:
        - The item at index is removed and its data buffer and node are freed.
        - This is equivalent to popIndex(self, index, false).

    Error:
        - If index is out of range, list->err.is is set and INDEX_OUT_OF_RANGE is recorded.

    @param index => Index of the element to remove.
*/
static void removeIndex(list* self, i64 index); 

/*
    Doc:
        Retrieve (copy) the item at the specified index without removing it.

    Behavior:
        - Allocates a new data buffer, copies the stored data into it, and returns the new item.
        - Caller is responsible for freeing the returned item's data buffer.
        - The internal node remains intact.

    Error:
        - If index is out of range, list->err.is is set and a sentinel item (data = NULL, dataLen = 0) is returned.

    @param index => Index of the element to get.
    @return item => A copy of the item at index (caller must free item.data).
*/
static item get(list* self, i64 index);

/*
    Doc:
        Search for the first occurrence of a value in the list.

    Behavior:
        - Compares each node's data to the provided buffer for dataLen bytes.
        - Returns the zero-based index of the first matching element.

    Return:
        - On success returns the index (usize).
        - If not found, returns (usize)-1 and sets list->err.is with VALUE_NOT_FOUND.

    @param data    => Pointer to the data to search for.
    @param dataLen => Number of bytes to compare.
    @return usize  => Index of the first match or (usize)-1 if not found.
*/
static usize search(list* self, void* data, usize dataLen);

/*
    Doc:
        Convert possibly-negative index to an absolute (inverted-friendly) index.

    Behavior:
        - If index >= 0 returns index.
        - If index < 0, interprets index as offset from the end (e.g., -1 refers to last element).
        - If the resulting index is out of range returns -1.

    @param index => Input index (may be negative).
    @return isize => Normalized non-negative index, or -1 if out of range.
*/
static isize getInvertedIndex(list* self, i64 index);


static _node* _createnode(void** databufferptr, usize datalen);
static item _createitem(void** databufferptr, usize datalen);

/*
    doc:
        free the memory allocated for a node and its associated data buffer.

    behavior:
        - frees node->item.data (if non-null) and then frees the node itself.
        - safe to call only for nodes that were allocated by _createnode.

    @param node => pointer to the node to free. if null the function is a no-op.
*/
static void _createNode_free(_node* node);

/*
    Doc:
        Free the memory allocated for an item created by _createItem or returned by get/append/etc.

    Behavior:
        - Frees item.data if non-NULL. Does not touch the item structure itself (it's passed by value).

    @param item => The item whose data buffer should be freed.
*/
static void _createItem_free(item item); 

/*
    Doc:
        Detach a node from its list without freeing the node or its data.

    Behavior:
        - Adjusts list->first, list->last and list->len appropriately.
        - Does not free the node or its item.data. Caller decides what to do with the node afterwards.

    @param self => Pointer to the list containing the node.
    @param node => Node to extract from the list (must be part of self).
*/
static void _extractNodeFromList(list* self, _node* node);

/*
    Doc:
        Extract the item stored in a node and return it as a standalone item.

    Behavior:
        - Creates and returns an item representing the node's data (ownership semantics follow internal conventions).
        - Does not free the node itself; caller should free node if needed.
        - Returned item contains a pointer to the node's data buffer (ownership transfer may apply depending on internal usage).

    @param node => Node from which to extract the item.
    @return item => The extracted item (may share or transfer ownership per internal semantics).
*/
static item _extractItemFromNode(_node* node);

/*
    Doc:
        Find the node at the specified index.

    Behavior:
        - Uses index optimization (start from head or tail) to locate the node quickly.
        - If index is out of range returns NULL and sets the list error state.

    @param self  => The list to search.
    @param index => Zero-based index of the node to find (may be negative if inverted indexing is supported).
    @return node* => Pointer to the node or NULL if not found/out of range.
*/
static _node* _findNodeByIndex(list* self, i64 index);

/*
    Doc:
        Optimize/normalize an index for traversal.

    Behavior:
        - Converts possible negative indices to positive offsets relative to list->len.
        - Chooses traversal direction heuristics (used by callers) and returns normalized index.
        - If index is out of range returns -1.

    @param self  => The list being indexed.
    @param index => The incoming index (may be negative).
    @return isize => Normalized index or -1 if out of range.
*/
static isize _optimizeIndex(list* self, i64 index);

/*
    Doc:
        Validate that the provided index is within the valid range for the list.

    Behavior:
        - If the index is out of range, sets list->err.is and sets the appropriate error code (INDEX_OUT_OF_RANGE).
        - Does not modify the list other than updating the error state.

    @param self  => The list against which to check the index.
    @param index => The index to validate.
*/
static void list_checkIndexInRange(list* self, i64 index);

// Implementation
#include <stdlib.h>
#include <string.h>

#include <types.h>
#include <vit/subsystems/linked_list.h>


list List(void) {
    return (list) {0};
}


_node* _list_createNode(void** dataBufferPtr, usize dataLen) {
    _node* new = malloc(sizeof(_node));
    if (!new) return null;
    *dataBufferPtr = malloc(dataLen);
    if (!(*dataBufferPtr)) free(new); return null;

    new->item.data = *dataBufferPtr;
    new->item.dataLen = dataLen;
    return new;
}


item list_append(list* self, u64 dataLen) {

    // create node & data buffer
    void* dataBuffer;
    _node* new = _list_createNode(&dataBuffer, dataLen);
    if (!new || !dataBuffer) self->err.code = list_INDEX_OUT_OF_RANGE; return (item) {0};
    new->next = null;;

    // Attach the node
    if (!self->len) {
        self->_first = new;
        new->prev = null;
    } else {
        self->_last->next = new;
        new->prev = self->_last;
    }
    self->_last = new;
    
    // update metadata
    self->len++;

    return (item) {
        .data = dataBuffer,
        .dataLen = dataLen
    };
}

item list_prepend(list* self, u64 dataLen) {
    
    // create node & data buffer
    void* dataBuffer;
    _node* new = _list_createNode(&dataBuffer, dataLen);
    if (!new || !dataBuffer) self->err.code = list_INDEX_OUT_OF_RANGE; return (item) {0};
    new->prev = null;
    
    // Attach the node
    if (!self->len) {
        self->_last = new;
        new->next = null;
    } else {
        new->next = self->_first;
        self->_first->prev = new;
    }
    self->_first = new;

    // update metadata
    self->len++;

    return (item) {
        .data = dataBuffer,
        .dataLen = dataLen
    };
}

item list_insert(list* self, u64 dataLen, i64 index, b8 replace) {
    /* Insert is only allowed within the current range (< len), or equal to len, in which case it will be appended */
    if (index >= 0) {
        if (index == self->len) return list_append(self, dataLen);
    } else {
        if (list_getInvertedIndex(self, index) == self->len) return list_append(self, dataLen);
    }

    _node* currentNode = _list_findNodeByIndex(self, index);
    if (self->err.is) return (item) {0};
    
    void* dataBuffer;
    _node* new = _list_createNode(&dataBuffer, dataLen);
    if (!new || !dataBuffer) self->err.code = list_INDEX_OUT_OF_RANGE; return (item) {0};

    // Override currentNode / push it forward depending on 'replace' parameter
    if (replace) {
        new->next = currentNode->next;
        new->prev = currentNode->prev;

        if (!currentNode->prev || !currentNode->next)
            self->_first = new;
        else 
            currentNode->prev->next = new;

        //! Old code something fishy ?? Anyways a simplified form is the above 4 lines
        /*
        currentNode->prev == null?
                ({list->first = new;})
            :   ({currentNode->prev->next = new;});
        currentNode->next == null?
                ({list->first = new;})
            :   ({currentNode->prev->next = new;});
        */
        //! ?
   } else {
        if (!currentNode->prev) self->_first = new; 
        new->next = currentNode;
        new->prev = currentNode->prev;
        currentNode->prev = new;
    }

    // update list metadata
    self->len++;

    return (item) {
        .data = dataBuffer,
        .dataLen = dataLen
    };
}

item list_pop(list* self, b8 toReturn) {
    /* If toReturn is true, the user will have to manually call free_item() function to free the item once it has been used */

    if (!self->len) self->err.code = list_POP_FROM_EMPTY_LIST; return (item) {0};
    
    // Rewire
    _node* toRemove = self->_last;
    _list_extractNodeFromList(self, toRemove);
    
    // update metadata
    self->len--;

    item ret;
    if (toReturn) {
        ret = _list_extractItemFromNode(toRemove);
    } else {
        ret = (item) {0};
    }
    _list_createNode_free(toRemove);

    return ret;
}

item list_popIndex(list* self, i32 index, b8 toReturn) {
    /* If toReturn is true, the user will have to manually call free_item() function to free the item once it has been used */

    _node* toRemove = _list_findNodeByIndex(self, index);
    if (self->err.is) return (item) {0};

    // Update metadata
    self->len--;
    
    item ret;
    if (toReturn) {
        ret = _list_extractItemFromNode(toRemove);
    } else {
        ret = (item) {0};
    }
    _list_createNode_free(toRemove);

    return ret;
}

void list_removeValue(list* self, void* data, u64 dataLen) {
    // Find the node
    _node* currentNode = self->_first;
    for (usize i = 0; i < self->len; i++) {
        if (dataLen != currentNode->item.dataLen) continue;
        if (!memcmp(currentNode->item.data, data, dataLen)) break;
        currentNode = currentNode->next;
    }
    // If value not found
    if (!currentNode) self->err.code = list_VALUE_NOT_FOUND; return;

    _list_extractNodeFromList(self, currentNode);
    _list_createNode_free(currentNode);

    // Update metadata
    self->len--;
}

void list_removeIndex(list* self, i64 index) {
    _node* node = _list_findNodeByIndex(self, index);
    if (self->err.is) return;

    _list_extractNodeFromList(self, node);
    _list_createNode_free(node);

    // Update metadata
    self->len--;
}

item list_get(list* self, i64 index) {

    _node* currentNode = _list_findNodeByIndex(self, index);
    if (self->err.is) return (item) {0};
    
    return currentNode->item;
}

u64 list_search(list* self, void* data, u64 dataLen) {
    /* O(n) Linear Search */
    _node* currentNode = self->_first;
    for (usize i = 0; i < self->len; i++) {
        if (currentNode->item.dataLen != dataLen) continue;
        if (!memcmp(currentNode->item.data, data, currentNode->item.dataLen)) return i;
        currentNode = currentNode->next;
    }
    return -1;
}

isize list_getInvertedIndex(list* self, i64 index) {
    return index > 0 ? -(self->len - index) : self->len + index;
}

isize _list_optimizeIndex(list* self, i64 index) {
    // Index optimization; if index is closer to the opposite side of the list (2 sides because -ive indexing is supported), invert it so that the list is traversed from the closer side, saving compute & time.
    if ((index > 0 && index > self->len / 2) || (index < -1 && llabs(index) > self->len / 2))
        return list_getInvertedIndex(self, index);   
    return index;
}

void _list_extractNodeFromList(list* self, _node* node) {
    // Unwire the node
    if (!node->prev) {
        self->_first = node->next;
    } else {
        node->prev->next = node->next;
    }
    if (!node->next) {
        self->_last = node->prev;
    } else {
        node->next->prev = node->prev;
    }
}

item _list_extractItemFromNode(_node* node) {
    // We do not need the item separately, because each node has one item only
    item ret = node->item;
    node->item = (item) {0};
    return ret;
}

void _list_createItem_free(item item) {
    if (!item.data) return; // Basically if the item struct is all 0 padded, return. Also if item.data is a nullptr, return; 
    free(item.data);
}

void _list_createNode_free(_node *node) {
    if (!node) return;
    _list_createItem_free(node->item);
    free(node);
}

void list_free(list* self) {
    // Free all the nodes from the heap
    if (!self || !self->len) return;
    _node* currentNode = self->_first;
    _node* nextNode;
    for (usize i = 0; i < self->len; i++) {
        nextNode = currentNode->next;
        currentNode->free(currentNode);
        currentNode = nextNode;
    }
}


_node* _list_findNodeByIndex(list* self, i64 index) {

    _list_checkIndexInRange(self, index);
    if (self->err.is) return null;

    index = _list_optimizeIndex(self, index);
    _node* currentNode;
    if (index >= 0) {
        // Traverse the list
        currentNode = self->_first;
        for (usize i = 0; i < index; i++) {
            currentNode = currentNode->next;
        }
    } else {
        // Traverse the list from the end
        currentNode = self->_last;
        for (isize i = -1; i > index; i--) {
            currentNode = currentNode->prev;
        }
    }

    return currentNode;
}

void _list_checkIndexInRange(list* self, i64 index) {
    // Check if index out of range
    if (index >= 0) {
        if (!(index < self->len)) self->err.code = list_INDEX_OUT_OF_RANGE; return;
    } else {
        if (!(llabs(index+1) < self->len)) self->err.code = list_INDEX_OUT_OF_RANGE; return;
    }
}