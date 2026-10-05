#include <stdlib.h>
#include <vit/subsystems/linked_list.h>
#include <types.h>

void _attachFuncPtrs(list* list) {
    list->free = free_list;
    list->append = append;
    list->prepend = prepend;
    list->insert = insert;
    list->pop = pop;
    list->popIndex = popIndex;
    list->removeIndex = removeIndex;
    list->removeValue = removeValue;
    list->get = get;
    list->search = search;
    list->getInvertedIndex = getInvertedIndex;
    list->clear = clear;
    list->_findNodeByIndex = _findNodeByIndex;
    list->_optimizeIndex = _optimizeIndex;
    list->_checkIndexInRange = _checkIndexInRange;
    list->_extractNodeFromList = _extractNodeFromList;
    list->_createNode = _createNode;
    list->createItem = createItem;
    list->_extractItemFromNode = _extractItemFromNode;
}

list* List(void) {
    list* self = malloc(sizeof(list));
    if (!self) return null;

    _attachFuncPtrs(self);
    self->len = 0;
    self->_first = null;
    self->_last = null;

    return self;
}

list* List_fromArray(const void* arr, usize dataLen, usize arrLen) {
    list* self = List();
    if (!self) return null;

    for (usize i = 0; i < arrLen; i++) {
        self->append(self, self->createItem(arr + (i * dataLen), dataLen));
    }
}