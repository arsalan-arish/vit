/* Doubly Linked List implementation */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <vit/subsystems/linked_list.h>
#include <types.h>


list* List(void) {
    list* self = malloc(sizeof(list));
    if (!self) return null;

    self->len = 0;
    self->_first = null;
    self->_last = null;
    self->err.is = false;

    // Attach the function ptrs
    self->free = free_list;
    self->insert = insert;
    self->append = append;
    self->prepend = prepend;
    self->removeIndex = removeIndex;
    self->removeValue = removeValue;
    self->get = get;
    self->search = search;
    self->getInvertedIndex = getInvertedIndex;
    self->clear = clear;
    self->pprint = pprint;
    self->createItem = createItem;

    self->_findNodeByIndex = _findNodeByIndex;
    self->_wireNode = _wireNode;
    self->_unwireNode = _unwireNode;

    self->_optimizeIndex = _optimizeIndex;
    self->_checkIndexInRange = _checkIndexInRange;

    self->_createNode = _createNode;
    self->_unwireItemFromNode = _unwireItemFromNode;

    return self;
}

list* List_fromArray(const u8* arr, usize dataLen, usize arrLen) {
    
    list* self = List();
    if (!self) return null;

    for (usize i = 0; i < arrLen; i++) {
        item* it = self->createItem(arr + (i * dataLen), dataLen);
        if (!it) return null;

        self->append(self, it);
        if (self->err.is) return null;
    }

    return self;
}

static void free_list(list* self) {
    if (!self->len) free(self); return;

    _node *currentNode = self->_first;
    _node* nextNode;
    for (usize i = 0; i < self->len; i++) {
        nextNode = currentNode->next;
        currentNode->free(currentNode);
        currentNode = nextNode;
    }

    free(self);
}

static void insert(list* self, item* it, isize index, b8 replace) {

    // Validate index
    self->_checkIndexInRange(self, index);
    b8 toAppend = llabs(index + 1) == self->len ? true : false;
    if (self->err.code == list_INDEX_OUT_OF_RANGE && !toAppend) return;

    // Create a node
    _node* new = self->_createNode(it);
    if (!new)  {
        self->err.is = true;
        self->err.code = list_HEAP_FAILURE; 
        return;
    }

    // Attach it
    if (replace && !toAppend) {
        _node* old = self->_findNodeByIndex(self, index);
        self->_unwireNode(self, old);
        old->free(old);
    }
    self->_wireNode(self, new, index);
}

static void append(list* self, item* it) {
    self->insert(self, it, self->len, false);

}

static void prepend(list* self, item* it) {
    self->insert(self, it, 0, false);
}

static item* removeIndex(list* self, isize index, b8 toReturn) {
    // Validate index
    self->_checkIndexInRange(self, index);
    if (self->err.is) return null;
    // Locate node
    _node* toRemove = self->_findNodeByIndex(self, index);
    // Unwire it
    self->_unwireNode(self, toRemove);
    // Extract item if needed
    item* it;
    if (toReturn) {
        it = self->_unwireItemFromNode(toRemove);
    } else {
        it = null;
    }
    // Free the node
    toRemove->free(toRemove);

    return it;
}
static void removeValue(list* self, item* it) {

    isize index = self->search(self, it);
    if (index == -1) {
        self->err.is = true;
        self->err.code = list_VALUE_NOT_FOUND;
        return;
    }
    _node* node = self->_findNodeByIndex(self, index);
    // Unwire node
    self->_unwireNode(self, node);
    // Free the node
    node->free(node);
}

static const item* get(list* self, isize index) {
    // Validate index
    self->_checkIndexInRange(self, index);
    if (self->err.is) return null;
    // Locate node
    _node* node = self->_findNodeByIndex(self, index);
    // Return the item pointer
    return node->item;
}

static isize search(list* self, item* it) {
    // O(n) Linear Search
    _node* currentNode = self->_first;
    for (usize i = 0; i < self->len; i++) {
        if (currentNode->item->dataLen != it->dataLen) {
            continue;
        }
        if (memcmp(currentNode->item->data, it->data, it->dataLen) == 0) {
            return i;
        }
        currentNode = currentNode->next;
    }
    return -1;
}

static isize getInvertedIndex(list* self, isize index) {
    return index > 0 ? -(self->len - index) : self->len + index;
}

static void clear(list* self) {
    // Traverse through list and free all nodes (copied from free_list function)
    _node *currentNode = self->_first;
    _node* nextNode;
    for (usize i = 0; i < self->len; i++) {
        nextNode = currentNode->next;
        currentNode->free(currentNode);
        currentNode = nextNode;
    }
    // Reset attributes
    self->len = 0;
    self->_first = null;
    self->_last = null;
    self->err.is = false;
}

static void pprint(list* self) {
    printf("\nLen => %zu\n", self->len);
}

static void _createItem_free(item* it) {
    free(it->data);
    free(it);
}

static item* createItem(const void* src, usize dataLen) {

    item* new = malloc(sizeof(item));
    if (!new) return null;
    new->data = malloc(dataLen);
    if (!new->data) free(new); return null;

    new->free = _createItem_free;
    new->dataLen = dataLen;
    memmove(new->data, src, dataLen);

    return new;
}


static _node* _findNodeByIndex(list* self, isize index) {
    index = self->_optimizeIndex(self, index);

    _node* currentNode;
    if (index >= 0) {
        currentNode = self->_first;
        for (usize i = 0; i < index; i++) {
            currentNode = currentNode->next;
        }
    } else {
        currentNode = self->_last;
        for (isize i = -1; i > index; i--) {
            currentNode = currentNode->prev;
        }
    }

    return currentNode;
}

static void _wireNode(list* self, _node* node, isize index) {
    if (index < 0) index = self->getInvertedIndex(self, index); // keep things +ive & simple
    
    if (index == 0) {
        if (!self->len) {
            self->_first = node;
            self->_last = node;
            node->next = null;
            node->prev = null;
        } else {
            node->next = self->_first;
            node->prev = null;
            self->_first->prev = node;
            self->_first = node;
        }
        
    } else if (index == self->len) {
        node->next = null;
        node->prev = self->_last;
        self->_last->next = node;
        self->_last = node;

    } else {
        _node* existingNode = self->_findNodeByIndex(self, index);
        node->prev = existingNode->prev;
        node->next = existingNode;
        existingNode->prev->next = node;
        existingNode->prev = node;
    }
    
    self->len++;
}

static void _unwireNode(list* self, _node* node) {
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

    self->len--;
}

static isize _optimizeIndex(list* self, isize index) {
    if 
    (
        (index > 0 && index > self->len / 2)  ||
        (index < -1 && llabs(index + 1) > self->len / 2)
    )
    {
        return self->getInvertedIndex(self, index);
    }    
    return index;
}

static void _checkIndexInRange(list* self, isize index) {
    if (index >= 0) {
        if (!(index < self->len)) self->err.code = list_INDEX_OUT_OF_RANGE; return;
    } else {
        if (!(llabs(index+1) < self->len)) self->err.code = list_INDEX_OUT_OF_RANGE; return;
    }
}

static void _createNode_free(_node* node) {
    // Below null check is an exception from normal behavior. Because in the internal API Design, an item can be legitimately null (technique to 'take' the item's ownership so a node does not free it)
    if (node->item != null) node->item->free(node->item); 
    free(node);
}

static _node* _createNode(item* it) {
    _node* new = malloc(sizeof(_node));
    if (!new) return null;

    new->item = it;
    new->next = null;
    new->prev = null;
    new->free = _createNode_free;

    return new;
}


static item* _unwireItemFromNode(_node* node) {
    item* toReturn = node->item;
    node->item = null;
    return toReturn;
}