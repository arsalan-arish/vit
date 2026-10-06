#include <stdlib.h>
#include <string.h>
#include <vit/subsystems/linked_list.h>
#include <types.h>


list* List(void) {
    list* self = malloc(sizeof(list));
    if (!self) return null;

    self->len = 0;
    self->_first = null;
    self->_last = null;

    // Attach the function ptrs
    self->free = free_list;
    self->insert = insert;
    self->append = append;
    self->prepend = prepend;
    self->removeIndex = removeIndex;
    self->removeValue = removeValue;
    self->pop = pop;
    self->popIndex = popIndex;
    self->get = get;
    self->search = search;
    self->getInvertedIndex = getInvertedIndex;
    self->clear = clear;

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

List_fromArray_result List_fromArray(const void* arr, usize dataLen, usize arrLen) {
    
    List_fromArray_result toReturn;    
    list* self = List();

    toReturn.status = Err;
    if (!self)         toReturn.data.err.code = List_fromArray_HEAP_FAIL;
    else if (!arr)     toReturn.data.err.code = List_fromArray_ARR_IS_NULL;
    else if (!dataLen) toReturn.data.err.code = List_fromArray_DATALEN_IS_ZERO;
    else if (!arrLen)  toReturn.data.err.code = List_fromArray_ARRLEN_IS_ZERO;
    return toReturn;

    for (usize i = 0; i < arrLen; i++) {
        //TODO: Make sure that if append and createItem functions raise error, handle it here
        self->append(self, self->createItem(arr + (i * dataLen), dataLen));
    }

    toReturn.status = Ok;
    toReturn.data.ret.list = self;
    return toReturn;
}

static void free_list(list* self) {
    if (!self) return;
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
    if (!self) return;

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

    /*
        if (replace) {
            if !toAppend
                findExistingNode at index
                unwire it
                free it
            wireNewNode at index
        } else {
            wireNewNode at index
        }
    */

    // update metadata
    self->len++;
}

//TODO: Design problem. I think '_node' has alot of its own logic hence a separate object shall be created for it
static void append(list* self, item* it) {
    if (!self) return; 

    // Create a node
    _node* node = self->_createNode(it);
    if (!node)  {
        self->err.is = true;
        self->err.code = list_HEAP_FAILURE; 
        return;
    }

    // Attach node


    // update metadata
    self->len++;
}

static void prepend(list* self, item* it) {

}


static void _createItem_free(item* it) {
    if (!it) return;

    free(it->data);
    free(it);
}

static item* createItem(const void* src, usize dataLen) {
    if (!src || !dataLen) return null;

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

}

static void _wireNode(list* self, _node* node) {

}

static void _unwireNode(list* self, _node* node) {

}

static isize _optimizeIndex(list* self, isize index) {

}

static void _checkIndexInRange(list* self, isize index) {
    if (index >= 0) {
        if (!(index < self->len)) self->err.code = list_INDEX_OUT_OF_RANGE; return;
    } else {
        if (!(llabs(index+1) < self->len)) self->err.code = list_INDEX_OUT_OF_RANGE; return;
    }
}

static void _createNode_free(_node* node) {
    if (!node) return;
    node->item->free(node->item);
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

